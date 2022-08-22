/*  Library for the HYT939 temperature and humidity sensor used with the ESP8266 and ESP32
    Datasheet can be found here: https://www.farnell.com/datasheets/3627289.pdf

    Application
    This sensor is used in high demand humidity and temperature measurement applications. It provides high accurycy measurement with a ruggedized design.

    Library by MaxQ22: Github https://github.com/MaxQ22/HYT939

    This file is licenced under the GNU GPLv3
*/

//include the necessary libraries, for the HYT939 and the wire (I2C) library
#include "HYT939.h"
#include <Wire.h>

void HYT939::Init(unsigned char SDA, unsigned char SCL)
{
  //As the pins are used for an I2C bus, switch on the pull ups, to stabilize the bus idle state
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);
  //Initialize the I2C communication
  Wire.begin(SDA, SCL);
}

unsigned char HYT939::Measure(void)
{
  //Allocate a data buffer to buffer the four bytes expected from the sensor, containing humidity and temperature raw values
  unsigned char data[4];
  unsigned char error = 0;    
   
  //Send only the address of the sensor with a write bit extension, to trigger a measurement
  Wire.beginTransmission(HYT_ADDR);
  Wire.endTransmission();
  
  //Delay, until the measurement is done
  unsigned long millis_start_wait = millis();
  while(millis() < millis_start_wait + 300)
  {
    //Wait until measurement is done
  }
  
  // Request 4 bytes of data, first two will be humidity, last two will be temperature
  Wire.requestFrom(HYT_ADDR, 4);

  //If the transfer was successfull, copy the four bytes obtained to the data buffer and calculate the scaled values
  if(Wire.available() == 4)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
 
    //Scale data to physical values
    Humidity = (float)(((data[0] & 0x3F)<<8) +  data[1]) * (100.0 / 16384.0);
    Temperature = ((float)((data[2]<<8) + (data[3] & 0xFC)) / 4.) * (165.0 / 16384.0) - 40;
  }
  else
  {
    error = 1; //If there was an error in the I2C transfer, return an error 
  }
  return error;
}

float HYT939::GetTemperature(void)
{
  //Just return the last measured temeprature
  return Temperature;
}

float HYT939::GetHumidity(void)
{
  //just return the last measured humidity
  return Humidity;
}
