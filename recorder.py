#!/usr/bin/env python

from __future__ import print_function
import sys
import argparse
import os.path
import time
from datetime import datetime
import calendar

try:
    import paho.mqtt.client as mqtt
except ImportError:
    print("Please install paho-mqtt", file=sys.stderr)
    sys.exit(1)

parser = argparse.ArgumentParser(description="Record sensor data to csv file")
parser.add_argument("-b", "--broker", required=True, help="MQTT broker")
parser.add_argument("-p", "--port", default=1883, type=int,
                    help="Port of the MQTT broker")
parser.add_argument("-s", "--sensor", required=True,
                    help="Base topic of the sensor")
parser.add_argument("-o", "--out", required=True, help="Output file")
parser.add_argument("-u", "--user", default=None,
                    help="Username for the MQTT broker")
parser.add_argument("-pw", "--password", default=None,
                    help="Password for the MQTT broker")
parser.add_argument("-t", "--value-timeout", default=5, type=int,
                    help="Maximum time in seconds between two values from different columns to be grouped into the same row")
parser.add_argument("-d", "--daily-file", action="store_true",
                    help="Store measurements in separate files for each day (date will be appended to filename)")
parser.add_argument("-v", "--verbose", action="store_true",
                    help="Print all incoming messages")

if len(sys.argv) == 1:
    parser.print_help(sys.stderr)
    sys.exit(1)

args = parser.parse_args()

temperature_ground_truth = None
temperature = None
humidity = None
last_received_time = 0


def on_connect(client, userdata, flags, rc):
    print("Connected!")
    print("Subscribing to sensor at {}/...".format(args.sensor))
    mqtt_client.subscribe(args.sensor + "/temperatureGroundTruth")
    mqtt_client.subscribe(args.sensor + "/temperature")
    mqtt_client.subscribe(args.sensor + "/humidity")


def on_disconnect(client, userdata, rc):
    print("Disconnected!")


def timeout_values():
    global temperature_ground_truth
    global temperature
    global humidity
    global last_received_time

    if time.time() - last_received_time > args.value_timeout:
        if args.verbose:
            print("Synchronized! Begining new row.")
        temperature_ground_truth = None
        temperature = None
        humidity = None
    last_received_time = time.time()


def on_message(client, userdata, msg):
    global temperature_ground_truth
    global temperature
    global humidity

    timeout_values()
    if args.verbose:
        print(msg.topic + " " + str(msg.payload))
    value = float(msg.payload)
    if msg.topic.endswith("/temperatureGroundTruth"):
        temperature_ground_truth = value
    elif msg.topic.endswith("/temperature"):
        temperature = value
    else:  # "/humidity"
        humidity = value
    if temperature_ground_truth != None and temperature != None and humidity != None:
        store_row([temperature_ground_truth, temperature, humidity])
        temperature_ground_truth = None
        temperature = None
        humidity = None


def store_row(row):
    utc_timestamp = calendar.timegm(datetime.utcnow().utctimetuple())
    if args.daily_file:
        create_current_file_if_not_exists()
    with open(get_filename(), "a") as file:
        file.write("{0},{1},{2},{3}\n".format(
            utc_timestamp, row[0], row[1], row[2]))


def get_filename():
    if args.daily_file:
        return args.out + "." + datetime.now().strftime("%Y-%m-%d")
    else:
        return args.out


def create_current_file_if_not_exists():
    if not os.path.isfile(get_filename()):
        with open(get_filename(), "w+") as file:
            file.write(
                "timestamp,temperature_ground_truth,temperature,humidity\n")


if not args.daily_file:
    create_current_file_if_not_exists()

mqtt_client = mqtt.Client()
mqtt_client.on_connect = on_connect
mqtt_client.on_message = on_message
mqtt_client.on_disconnect = on_disconnect

print("Connecting to {0}:{1}...".format(args.broker, args.port))
mqtt_client.username_pw_set(args.user, args.password)
try:
    mqtt_client.connect(args.broker, args.port)
except:
    print("Error! Could not connect to {0} on Port {1}".format(
        args.broker, args.port), file=sys.stderr)
    exit(1)
try:
    mqtt_client.loop_forever()
except KeyboardInterrupt:
    pass
