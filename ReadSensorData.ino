#include <Adafruit_Sensor.h>
#include <DHT.h>

#include "config.h"

#if SIMULATED_DATA

void InitSensor()
{
    // use SIMULATED_DATA, no sensor need to be inited
}

float ReadTemperature()
{
    return random(20, 30);
}

float ReadHumidity()
{
    return random(30, 40);
}

#else

static DHT dht(DHT_PIN, DHT_TYPE);

/*****************************************************************************
 * Function to initialize the DHT22 sensor.
 *****************************************************************************/
void InitSensor()
{
    dht.begin();
}

/*****************************************************************************
 * Function to read temperature on the DHT22 sensor (through OneWire).
 * The function retries in case a temperature read fails.
 *****************************************************************************/
float ReadTemperature()
{
    float temperature = dht.readTemperature();

    while (std::isnan(temperature))
    {
        delay(200);
        temperature = dht.readTemperature();
    }

    return temperature;

}
#endif

