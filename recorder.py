#!/usr/bin/env python

from __future__ import print_function
import sys
import argparse
import os.path
import time
from datetime import datetime

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
parser.add_argument("-d", "--daily-file", action="store_true",
                    help="Store records in separate files for each day (date will be appended to filename)")
parser.add_argument("-v", "--verbose", action="store_true",
                    help="Print all incoming messages")

if len(sys.argv) == 1:
    parser.print_help(sys.stderr)
    sys.exit(1)

args = parser.parse_args()

temperatureGroundTruthValues = []
temperatureValues = []
humidityValues = []


def on_connect(client, userdata, flags, rc):
    print("Connected!")
    print("Subscribing to sensor at {}/...".format(args.sensor))
    mqttClient.subscribe(args.sensor + "/temperatureGroundTruth")
    mqttClient.subscribe(args.sensor + "/temperature")
    mqttClient.subscribe(args.sensor + "/humidity")


def on_disconnect(client, userdata, rc):
    print("Disconnected!")
    exit(1)


def on_message(client, userdata, msg):
    if args.verbose:
        print(msg.topic + " " + str(msg.payload))
    value = float(msg.payload)
    if msg.topic.endswith("/temperatureGroundTruth"):
        temperatureGroundTruthValues.append(value)
    elif msg.topic.endswith("/temperature"):
        temperatureValues.append(value)
    else:  # "/humidity"
        humidityValues.append(value)
    if len(temperatureGroundTruthValues) > 0 and len(temperatureValues) > 0 and len(humidityValues) > 0:
        record = [temperatureGroundTruthValues.pop(
            0), temperatureValues.pop(0), humidityValues.pop(0)]
        store_record(record)


def store_record(record):
    if args.daily_file:
        create_records_file_if_not_exists()
    with open(get_filename(), "a") as file:
        file.write("{0},{1},{2},{3}\n".format(
            int(time.time()), record[0], record[1], record[2]))


def get_filename():
    if args.daily_file:
        return args.out + "." + datetime.now().strftime("%Y-%m-%d")
    else:
        return args.out


def create_records_file_if_not_exists():
    if not os.path.isfile(get_filename()):
        with open(get_filename(), "w+") as file:
            file.write(
                "timestamp,temperatureGroundTruth,temperature,humidity\n")


if not args.daily_file:
    create_records_file_if_not_exists()

mqttClient = mqtt.Client()
mqttClient.on_connect = on_connect
mqttClient.on_message = on_message
mqttClient.on_disconnect = on_disconnect

print("Connecting to {0}:{1}...".format(args.broker, args.port))
mqttClient.username_pw_set(args.user, args.password)
try:
    mqttClient.connect(args.broker, args.port)
except:
    print("Error! Could not connect to {0} on Port {1}".format(
        args.broker, args.port), file=sys.stderr)
    exit(1)
try:
    mqttClient.loop_forever()
except KeyboardInterrupt:
    pass
