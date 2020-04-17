#ifndef _HPDAQ_H
#define _HPDAQ_H

#include <Arduino.h>
#include <Wire.h>

struct hpdaqConfig {
    union {
        uint8_t reg;
        struct {
            unsigned gain:2;
            unsigned rate:2;
            unsigned mode:1;
            unsigned channel:2;
            unsigned rdy:1;
        } __attribute__((packed));
    } __attribute__((packed));
} __attribute__((packed));

class HPDAQ {
    private:
        uint8_t _address;
        uint8_t _gain;
        struct hpdaqConfig _config;
        int16_t _adcReading;

        void updateConfig();
        void readData();
    
    public:
        static const uint8_t GAIN_1 = 0;
        static const uint8_t GAIN_2 = 1;
        static const uint8_t GAIN_4 = 2;
        static const uint8_t GAIN_8 = 3;

        static const uint8_t MODE_SINGLE = 0;
        static const uint8_t MODE_CONTINUOUS = 1;

        static const uint8_t RES_12 = 0;
        static const uint8_t RES_14 = 1;
        static const uint8_t RES_16 = 2;

        static const uint8_t _baseAddress = 0b1101000;

        HPDAQ(uint8_t a) : _address(a) {}
        void begin();
        void setGain(uint8_t);
        void setResolution(uint8_t);
        void setChannel(uint8_t);
        void setMode(uint8_t);
        int16_t analogRead(uint8_t);
        double analogReadVoltage(uint8_t);

        void startConversion(uint8_t);
        int16_t fetchResult();
        double fetchResultVoltage();

        void blockSample(uint8_t, int16_t *buffer, int num);
        void blockSample(uint8_t, double *buffer, int num);

    
};

#endif
