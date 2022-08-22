/*  Library for the HYT939 temperature and humidity sensor used with the ESP8266 and ESP32
    Datasheet can be found here: https://www.farnell.com/datasheets/3627289.pdf

    Application
    This sensor is used in high demand humidity and temperature measurement applications. It provides high accurycy measurement with a ruggedized design.

    Library by MaxQ22: Github https://github.com/MaxQ22/HYT939

    This file is licenced under the GNU GPLv3
*/

#define HYT_ADDR 0x28

class HYT939
{
  public:
    void Init(unsigned char SDA, unsigned char  SCL);
    unsigned char Measure(void);
    float GetTemperature(void);
    float GetHumidity(void);
  private:  
    float Humidity;
    float Temperature; 
};
