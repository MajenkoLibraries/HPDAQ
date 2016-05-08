High Precision Data Acquisition Shield Library
==============================================

Usage:
------
````C++
#include <HPDAQ.h>

HPDAQ daq(0);
````

You can specify the board address 0-7 in the constructor. Current shields
only support address 0.

Read a raw sample:

````C++
int val = daq.analogRead(channel);
// channel = 0 to 3
````
Read a voltage:

````C++
double volts = daq.analogReadVoltage(channel);
````
Select the resolution:

````C++
daq.setResolution(HPDAQ::RES_12);
daq.setResolution(HPDAQ::RES_14);
daq.setResolution(HPDAQ::RES_16);
````
Select the pre-amp gain:

````C++
daq.setGain(HPDAQ::GAIN_1);
daq.setGain(HPDAQ::GAIN_2);
daq.setGain(HPDAQ::GAIN_4);
daq.setGain(HPDAQ::GAIN_8);
````
Start a background sample:

````C++
daq.startConversion(channel);
````
Retrieve the results of the last background conversion:

````C++
int val = daq.fetchResult();
double volts = daq.fetchResultVoltage();
// These functions block until the conversion is complete if
// not already complete.
````

