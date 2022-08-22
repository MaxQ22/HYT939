/*  Example for the HYT939 temperature and humidity sensor used with the ESP8266 and ESP32
    Datasheet can be found here: https://www.farnell.com/datasheets/3627289.pdf

    Application
    This sensor is used in high demand humidity and temperature measurement applications. It provides high accurycy measurement with a ruggedized design.

    Library by MaxQ22: Github https://github.com/MaxQ22/HYT939

    This file is licenced under the GNU GPLv3
*/

//include the HYT939 library
#include "HYT939.h"

//define the I2C pins either for the ESP32 or the ESP8266
#ifdef ESP32

  #define HYT_SDA 23
  #define HYT_SCL 22 

#else

  #define HYT_SDA D2
  #define HYT_SCL D1

#endif

//Generate an instance of the HYT939 class
HYT939 HYT;

void setup()
{
  //As the sensor values are transmitted via the serial connection, initlaize it
  Serial.begin(9600);

  //Initialize the HYT939 sensor, which initializes the I2C bus, connecting the ESP and the sensor
  HYT.Init(HYT_SDA, HYT_SCL);
}

void loop()
{
  //Allocate some variables to store the sensor values
  float Temperature = 0;
  float Humidity = 0;
  unsigned char error;

  //Send a measure command to the HYT939 to measure temperature and humidity
  error = HYT.Measure();
  
  if(error != 0)
  {
    //If there was an error during the measurement, print an error message
    Serial.println("An error occured during the measurement");
  }
  else
  {
    //If there was no error during the measurement, retrive the measured values and print them to the serial monitor
    Temperature = HYT.GetTemperature();
    Humidity = HYT.GetHumidity();
    Serial.print("Temperature: ");
    Serial.print(Temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(Humidity);
    Serial.println(" %"); 
  }

  delay(1000);
}
