#include <ArduinoJson.h>

/*****************************************************************************
 * Function to send a DHT22 sensor message to an IoTHub.
 * From here we regularly send temperature readings to the IoTCentral app.
 *****************************************************************************/
void SendSensorMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, int msgCount)
{
    // wrap a temperature reading into JSON
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root[JSON_DEVICE] = DEVICE_ID;
    root[JSON_MESSAGEID] = msgCount;
    root[JSON_TEMPERATURE] = ReadTemperature();

    // send to IoTHub
    char messagePayload[MESSAGE_MAX_LEN];
    root.printTo(messagePayload, MESSAGE_MAX_LEN);
    SendMessage(iotHubClientHandle, messagePayload);
}


void SendDeviceLocation(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle)
{
    // wrap the location into JSON and send as reported property
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root[JSON_DEVICELOC] = DEVICE_LOCATION;
    
    char messagePayload[MESSAGE_MAX_LEN];
    root.printTo(messagePayload, MESSAGE_MAX_LEN);
    SendDeviceProperty(iotHubClientHandle, messagePayload);
}

