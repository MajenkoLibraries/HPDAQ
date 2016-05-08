/*
 * Copyright (c) 2016, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <HPDAQ.h>

HPDAQ daq(0);

void setup() {
    daq.begin();
    daq.setResolution(HPDAQ::RES_16);
    daq.setGain(HPDAQ::GAIN_1);
    Serial.begin(115200);

    // Start sampling from channel 0
    daq.startConversion(0);
}

void loop() {
    // Get the result of the channel 0 read
    double v1 = daq.fetchResultVoltage();
    // Start a conversin of channel 1
    daq.startConversion(1);
    // While it's converting print the channel 0 value to serial.
    Serial.print(v1, 8);
    Serial.print("    ");

    // Grab the new channel 1 value
    double v2 = daq.fetchResultVoltage();
    // and start converting channel 2
    daq.startConversion(2);
    // While it's converting print channel 1's value to serial.
    Serial.print(v2, 8);
    Serial.print("    ");

    // and so on
    double v3 = daq.fetchResultVoltage();
    daq.startConversion(3);
    Serial.print(v3, 8);
    Serial.print("    ");

    double v4 = daq.fetchResultVoltage();
    // Back to channel 0.
    daq.startConversion(0);
    Serial.println(v4, 8);
    delay(100);
}
