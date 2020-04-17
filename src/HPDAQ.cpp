#include <HPDAQ.h>

void HPDAQ::begin() {
    Wire.begin();
    _config.reg = 0;
    updateConfig();
}

void HPDAQ::updateConfig() {
    Wire.beginTransmission(_address + _baseAddress);
    Wire.write(_config.reg);
    Wire.endTransmission();
}

void HPDAQ::setGain(uint8_t g) {
    _config.gain = g;
    updateConfig();
}

void HPDAQ::setResolution(uint8_t r) {
    _config.rate = r;
    updateConfig();
}

void HPDAQ::setMode(uint8_t m) {
    _config.mode = m;
    updateConfig();
}

void HPDAQ::setChannel(uint8_t c) {
    _config.channel = c;
    updateConfig();
}

int16_t HPDAQ::analogRead(uint8_t c) {
    _config.mode = MODE_SINGLE;
    _config.channel = c;
    _config.rdy = 1;
    updateConfig();
    // Conversion is now happening
    while (_config.rdy == 1) {
        readData();
    }
    return _adcReading;
}

void HPDAQ::readData() {
    Wire.requestFrom(_address + _baseAddress, 3);
    uint8_t hi = Wire.read();
    uint8_t low = Wire.read();
    _config.reg = Wire.read();
    _adcReading = hi << 8 | low;
#if defined(DEBUG)
    Serial.printf("Status: %c %d %c %d %d\r\n", 
        _config.rdy ? 'R' : 'r',
        _config.channel,
        _config.mode ? 'C' : 'S',
        _config.rate,
        _config.gain    
    );
#endif
}

double HPDAQ::analogReadVoltage(uint8_t c) {
    double val = analogRead(c);
    double lsb = 0;
    double gain = 1;
    switch (_config.rate) {
        case RES_12: lsb = 0.001; break;
        case RES_14: lsb = 0.00025; break;
        case RES_16: lsb = 0.0000625; break;
    }
    switch (_config.gain) {
        case GAIN_1: gain = 1; break;
        case GAIN_2: gain = 2; break;
        case GAIN_4: gain = 4; break;
        case GAIN_8: gain = 8; break;
    }

    double out = val * (lsb / gain);
    return out;
}

void HPDAQ::startConversion(uint8_t c) {
    _config.mode = MODE_SINGLE;
    _config.channel = c;
    _config.rdy = 1;
    updateConfig();
}

int16_t HPDAQ::fetchResult() {
    // Conversion is now happening
    while (_config.rdy == 1) {
        readData();
    }
    return _adcReading;
}

double HPDAQ::fetchResultVoltage() {
    double val = fetchResult();
    double lsb = 0;
    double gain = 1;
    switch (_config.rate) {
        case RES_12: lsb = 0.001; break;
        case RES_14: lsb = 0.00025; break;
        case RES_16: lsb = 0.0000625; break;
    }
    switch (_config.gain) {
        case GAIN_1: gain = 1; break;
        case GAIN_2: gain = 2; break;
        case GAIN_4: gain = 4; break;
        case GAIN_8: gain = 8; break;
    }

    double out = val * (lsb / gain);
    return out;
}

void HPDAQ::blockSample(uint8_t c, int16_t *buffer, int num) {
    _config.mode = MODE_CONTINUOUS;
    _config.channel = c;
    _config.rdy = 1;
    updateConfig();
    for (int sno = 0; sno < num; sno++) {
        while (_config.rdy == 1) {
            readData();
        }
        buffer[sno] = _adcReading;
        readData();
    }
    _config.mode = MODE_SINGLE;
    updateConfig();
}

void HPDAQ::blockSample(uint8_t c, double *buffer, int num) {

    _config.mode = MODE_CONTINUOUS;
    _config.channel = c;
    _config.rdy = 1;
    updateConfig();

    double lsb = 0;
    double gain = 1;
    switch (_config.rate) {
        case RES_12: lsb = 0.001; break;
        case RES_14: lsb = 0.00025; break;
        case RES_16: lsb = 0.0000625; break;
    }
    switch (_config.gain) {
        case GAIN_1: gain = 1; break;
        case GAIN_2: gain = 2; break;
        case GAIN_4: gain = 4; break;
        case GAIN_8: gain = 8; break;
    }
    double mul = lsb / gain;

    for (int sno = 0; sno < num; sno++) {
        readData();
        while (_config.rdy == 1) {
            readData();
        }
        double val = _adcReading;

        double out = val * mul;
        buffer[sno] = out;
    }
    _config.mode = MODE_SINGLE;
    updateConfig();
}


