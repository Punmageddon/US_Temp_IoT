#define INITIALIZATION_LED LED_BUILTIN

// I2C (Lolin NodeMCU aka "NodeMCU V3")
#define I2C_SDA D2
#define I2C_SCL D1

// SPI (On Lolin NodeMCU aka "NodeMCU V3": CLK = D5, MISO = D6, MOSI = D7)
#define SPI_CHIP_SELECT_SD D8

#define ENABLE_SDCARD // Remove to disable
#define FILENAME "/sensor.csv"

#define ENABLE_WIFI_MQTT // Remove to disable

// WiFi
#define WIFI_SSID "**********"
#define WIFI_PASSWORD "**********"

// MQTT
#define MQTT_BROKER "test.mosquitto.org"
#define MQTT_USER NULL
#define MQTT_PASSWORD NULL
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "**********"
#define MQTT_BASE_TOPIC "**********"
