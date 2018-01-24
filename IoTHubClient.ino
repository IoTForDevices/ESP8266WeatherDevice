static WiFiClientSecure sslClient; // for ESP8266

/*****************************************************************************
 * Function to receive the message delivered callback from an IoTHub.
 *****************************************************************************/
static void SendCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result, void *userContextCallback)
{
    if (IOTHUB_CLIENT_CONFIRMATION_OK == result)
    {
        Serial.println("Message sent to Azure IoT Hub");
        BlinkLED();
    }
    else
    {
        Serial.println("Failed to send message to Azure IoT Hub");
    }

    messagePending = false;
}

/*****************************************************************************
 * Function to send a message to an IoTHub.
 *****************************************************************************/
static void SendMessage(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *buffer)
{
    IOTHUB_MESSAGE_HANDLE messageHandle = IoTHubMessage_CreateFromByteArray((const unsigned char *)buffer, strlen(buffer));
    if (messageHandle == NULL)
    {
        Serial.println("Unable to create a new IoTHubMessage.");
    }
    else
    {
        Serial.printf("Sending message: %s.\r\n", buffer);
        if (IoTHubClient_LL_SendEventAsync(iotHubClientHandle, messageHandle, SendCallback, NULL) != IOTHUB_CLIENT_OK)
        {
            Serial.println("Failed to hand over the message to IoTHubClient.");
        }
        else
        {
            messagePending = true;
            Serial.println("IoTHubClient accepted the message for delivery.");
        }
        IoTHubMessage_Destroy(messageHandle);
    }
}

/*****************************************************************************
 * Function to send a property to an IoTHub.
 *****************************************************************************/
static void SendDeviceProperty(IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle, char *payLoad)
{
      Serial.printf("Sending message: %s.\r\n", payLoad);

      if (IoTHubClient_LL_SendReportedState(iotHubClientHandle, (const unsigned char*)payLoad, strlen(payLoad), NULL, NULL) != IOTHUB_CLIENT_OK)
      {
          Serial.println("Failed to hand over the device property to IoTHubClient.");
      }
      else
      {
          // messagePending = true;
          Serial.println("IoTHubClient accepted the property for delivery.");
      }
}

