/*****************************************************************************
 * IoTCentral demo device, implementing a weather station, making use of a
 * DHT22 on pin 2 of a Feather Huzzah ESP8622.
 * 
 * - The device measures temperature only with a fixed interval.
 * - The IoT Central app contains a property for the device location, to be set
 *   by the device
 * - The IoT Central app contains an high temperature alarm rule, completely
 *   transparrent for the device.
 *   
 * Communication with the IoTCentral app's integrated IoTHub is done with the 
 * Azure IoT Device SDK (iothub_client). See https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-devguide-sdks.
 *
 * This sketch uses the following Arduino Libraries:
 * - AzureIoTHub
 * - AzureIoTUtility
 * - AzureIoProtocol_MQQT
 * - ESP8266WiFi
 * - DHT Sensor Library
 * - Adafruit Unified Sensor
 * - ArduinoJson
 * 
 * NOTE: The order in which the header files are included is critical!
 *****************************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>

#include <AzureIoTHub.h>
#include <AzureIoTUtility.h>
#include <AzureIoTProtocol_MQTT.h>

#include "credentials.h"
#include "config.h"

static const char *connectionString = IOTHUB_CS;
static const char *ssid = SSID;
static const char *pass = PASS;
static const char *deviceLocation = DEVICE_LOCATION;

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
static int interval = 10;   // time in seconds
static int messageCount = 1;

static bool messagePending = false;

/*****************************************************************************
 * Function to initialize serial communication at 115200 Baud
 * This function can also be used to enable / disable debug output. This
 * is especially useful when diagnosing WiFi network issues.
 *****************************************************************************/
void InitSerial()
{
    Serial.begin(115200);
    delay(10);
    Serial.setDebugOutput(false);
}

/*****************************************************************************
 * Function to initialize WiFi on the Feather Huzzah ESP8622
 * 
 * NOTE: The underlying library has difficulties connecting with SSIDs with
 * special characters like '. That's why it makes sense to change the default
 * SSID from a iPhone personal hotspot.
 *****************************************************************************/
void InitWiFi()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to WiFi ");

    WiFi.begin(ssid, pass);
    int status = WL_DISCONNECTED;
    while(status != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
        status = WiFi.status();
    }

    Serial.println();
    
    Serial.println("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

/*****************************************************************************
 * Function to set the right time to the Feather Huzzah.
 * 
 * NOTE: Time is critical for the Azure IoT SDK!
 *****************************************************************************/
void InitTime()
{
    time_t epochTime;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true)
    {
        epochTime = time(NULL);

        if (epochTime == 0)
        {
            Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        }
        else
        {
            Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
            break;
        }
    }
}

/*****************************************************************************
 * Function to blink a LED for 500 msec
 *****************************************************************************/
void BlinkLED()
{
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
}

/*****************************************************************************
 * Function to connect to the IoTCentral app's IoTHub
 *****************************************************************************/
void InitIoTHubClient()
{
    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        Serial.println("Failed on IoTHubClient_CreateFromConnectionString.");
        ESP.restart();
    }
    SendDeviceLocation(iotHubClientHandle);
}

/*****************************************************************************
 * Function to initialize the device application
 *****************************************************************************/
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    InitSerial();
    InitWiFi();
    InitTime();
    InitSensor();
    InitIoTHubClient();
}

/*****************************************************************************
 * Function hosting the device control loop.
 * From here we regularly send temperature readings to the IoTCentral app.
 *****************************************************************************/
void loop()
{
    int connectionStatus = WiFi.status();
    
    if (!messagePending && connectionStatus == WL_CONNECTED)
    {
        SendSensorMessage(iotHubClientHandle, messageCount);
        messageCount++;
    }
    else if (connectionStatus != WL_CONNECTED)
    {
        // try to reset the ESP8266, because it typically can't recover
        ESP.restart();
    }
    IoTHubClient_LL_DoWork(iotHubClientHandle);
    delay(messagePending ? 1000 : interval * 1000);
}
