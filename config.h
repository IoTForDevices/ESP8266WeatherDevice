/*****************************************************************************
 * Physical device information for board and sensor
 * Modify these settings for different pins and keep unique / device
 * 
 * NOTE: There is a tight relation between measurements and settings inside
 * the device app and their corresponding definitions in the IoTCentral app.
 * 
 *****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#define DEVICE_ID "SimpleWeatherStationESP8266-1.0.0"
#define DEVICE_VERSION_100 true
#define DEVICE_LOCATION "Home Office"

#define DHT_TYPE DHT22

// Pin layout configuration
#define LED_PIN 0
#define DHT_PIN 2

// Measurements and Settings
// These values are the json object names and should correspond to the Measurements and Settings as defined in the IoT Central solution.
#define JSON_TEMPERATURE  "dht22_Temperature"
#define JSON_DEVICE       "deviceId"
#define JSON_MESSAGEID    "messageId"
#define JSON_DEVICELOC    "device_location"
  
// If don't have a physical DHT sensor, can send simulated data to IoT hub
#define SIMULATED_DATA false


// SSID and SSID password's length should < 32 bytes (see http://serverfault.com/a/45509)
#define SSID_LEN 32
#define PASS_LEN 32

#define CONNECTION_STRING_LEN 256
#define MESSAGE_MAX_LEN 256

#endif
