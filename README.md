# HYT 939 Library for the ESP 8266 and ESP32

The HYT939 is a high accuracy, highly linear, stable temeprature and humidity sensor by IST (Innovative Sensor Technology). The sensor can sense a temperature from -40 °C to +125 °C and a relative humidity of 0 % to 100 %. It can operate with a supply voltage from 2.7 V to 5.5 V and is therefore suitable for usage with an ESP chip. This library works for both the ESP8266 and the ESP32 chip with the Arduino core. The datasheet of the MQ135 gas sennsor can be found [here](https://www.farnell.com/datasheets/3627289.pdf).

![Sensor Picture](https://www.ist-ag.com/sites/default/files/styles/x_large_full_column/public/2022-04/Humidity%20Module%20HYT939.jpg.webp?itok=Y3esNvcR)

## Usage

### Hardware Setup
To setup the hardware, connect the following pins of the HYT939 sensor to the following pins on the ESP8266 or ESP32

HYT939 | ESP8266 | ESP32
--- | --- | ---
Pin 1 SCL | D1 | 22
Pin 2 VCC | 3V3 | 3V3
Pin 3 GND | GND | GND
Pin 4 SDA | D2 | 23

![HYT939 Pinout](/HYT939_Pinout.PNG)

### Library Setup
To use the library, copy the two files HYT939.cpp and HYT939.h to your project directory. 
The first step in the code to use the library is to include the necessary header file:

```cpp
#include "HYT939.h"
```

As the HYT939 sensor has a I2C interface, we need to select the pins, it is connected to. For the ESP8266 this is:

```cpp
#define HYT_SDA D2
#define HYT_SCL D1 
```
For usage with the ESP32, the pin names need to be changed to:

```cpp
#define HYT_SDA 23
#define HYT_SCL 22 
```
This is the only change, that needs to be made between the ESP8266 and the ESP32, as the Arduino core abstracts all other functions.

The HYT939 library is basically a C++ class, namely the HYT939 class. Therefore, the next step is to define an instance of this class:

```cpp
HYT939 HYT;
```

In your setup() function you can call the Init function of the class. This will initialise the I2C communication. 

```cpp
HYT.Init(HYT_SDA, HYT_SCL);
```

As the HYT939 sensor itself does not need any initialization, this will not do anything else. So if you want to intialize the I2C communication yourself (e.g. because you have other devices on the same I2C bus), you can do so by calling the wire functions yourself:

```cpp
pinMode(SDA, INPUT_PULLUP);
pinMode(SCL, INPUT_PULLUP);
Wire.begin(SDA, SCL);
```
If the I2C initialization is done "manually", do not call the HYT939::Init() function.

Now all that is left to do is to command the HYT939 to take a measurement via the Measure() function and retrive the values for temperature and humidity inside the loop() function:

```cpp
float Temperature = 0;
float Humidity = 0;
HYT.Measure();
Temperature = HYT.GetTemperature();
Humidity = HYT.GetHumidity();
```

### Example
There is an example called HYT939.ino in the project directory. This example runs on both, ESP8266 and ESP32 boards. It showcases the steps shown above and can be used to check the hardware setup. It is putting out the temperature and humidity via the serial interface at a baud rate of 9600 baud.

## Theory of Operation
### I2C Transfer

The HYT939 measures the humidity via a cpacitive polymer element and the temperature via a integrated PTAT (proportional to absolute temperature) element. The values are then analog to digital converted on board the sensor. The sensor then transmits the values via a I2C interface. A german version of the I2C interface description can be found [here](https://cdn-reichelt.de/documents/datenblatt/E801/DS_HYT_PB_D.pdf). Basically the sensor waits for a measurement command, after the command is issued, it will measure temperature and humidity, which can then be polled via the data fetch command.
The standard I2C address of the sensor is 0x28. The measurement is triggered by a one byte transfer to the sensor, containing the sensor`s I2C address and the read/write bit set to write (0b0). 

After the measurement command, the sensor driver waits for 300 milliseconds, which is the maximum measure time. Then the data fetch command is started by requesting 4 bytes from the sensor. This request is triggered by writing a byte, containing the sensors address and the read/write byte set to read. The first two bytes send by the sensor contain the humidity value and the last two bytes contain the temperature value.

### Value Scaling 
The first two bits in the first byte are status bits, which are not used by this library. They are masked out. Then, the remaining six bits of the first byte and the second byte are joined together to obtain the measured value rFraw. Then, the value is scaled to float via the formula given in the I2C description: 

'''
rF[5] = 100 / 2^14 *rFraw
'''

The same approach is done for the temperature. The third byte send by the sensor contains the eight most significant bits of the raw temeprature value, and the 4th byte contains the six least significant bits of the raw temperature value. The last two bits in that byte carry no information and shall not be used and are masked out. Then, the raw value is converted to teh actual temperature via the formula given in the I2C description:

'''
T[°C] = 165 / 2^14 * Traw -40
'''
