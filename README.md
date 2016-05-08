High Precision Data Acquisition Shield Library
==============================================

Usage:
------

    #include <HPDAQ.h>

    HPDAQ daq(0);

You can specify the board address 0-7 in the constructor. Current shields
only support address 0.

* Read a raw sample:

    int val = daq.analogRead(channel);
    // channel = 0 to 3

* Read a voltage:

    double volts = daq.analogReadVoltage(channel);

* Select the resolution:

    daq.setResolution(HPDAQ::RES_12);
    daq.setResolution(HPDAQ::RES_14);
    daq.setResolution(HPDAQ::RES_16);

* Select the pre-amp gain:

    daq.setGain(HPDAQ::GAIN_1);
    daq.setGain(HPDAQ::GAIN_2);
    daq.setGain(HPDAQ::GAIN_4);
    daq.setGain(HPDAQ::GAIN_8);

* Start a background sample:

    daq.startConversion(channel);

* Retrieve the results of the last background conversion:

    int val = daq.fetchResult();
    double volts = daq.fetchResultVoltage();
    // These functions block until the conversion is complete if
    // not already complete.


