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

/* Wheatstone Bridge Thermistor Temperature Display
 * 
 * This is designed to run on a chipKIT board using the DisplayCore Library
 * https://github.com/MajenkoLibraries/DisplayCore
 * 
 * Connect a thermistor in a well-balanced Wheatstone Bridge to channel 0. Set the
 * values below to the right settings for your bridge.
 * 
 * You should then get a very high sensitivity temperature graph that is able to
 * display the tinyest of anbient temperature changes. Even bringing your finger
 * within a few inches of the thermistor registers a big change in temperature.
 */

#include <HPDAQ.h>
#include <ILI9341.h>

HPDAQ daq(0);
ILI9341 tft(PINS_8BIT_SHIELD);

// Wheatstone Bridge values
const float R1 = 100000; // Top left
const float R2 = 147000; // Bottom Left
const float R3 = 100000; // Top Right
const float Ve = 3.3333; // Excitation voltage

const float thermistorNormal = 140000; // Ambient temperature resistance
const float thermistorBeta = 3950;     // Beta value
const float thermistorAmbient = 25;    // Ambient temperature for restance above

// Screen mid point temperature
const int midTemp = 25;
// How many degrees above and below the mid point
const int tempsPerHalf = 2;

float last;

void setup() {
	daq.begin();

    // Highest resolution and sensitivity possible
	daq.setResolution(HPDAQ::RES_16);
	daq.setGain(HPDAQ::GAIN_8);
 
	tft.initializeDevice();
	tft.setRotation(1);
	tft.fillScreen(Color::Black);

    // Start an initial conversion in the background
	daq.startConversion(0);
    last = -999999;
}


void loop() {
	int h = tft.getHeight();
	int w = tft.getWidth();
	int mid = h / 2;
	double scale = mid / tempsPerHalf;
	tft.drawLine(0, 0, 0, h, Color::Black);

	for (int i = 0; i < w - 1; i++) {
		// Get the most recent result
		float now = daq.fetchResultVoltage();
		// and start a new conversion while we do the drawing
		daq.startConversion(0);

        // Draw the grid lines and temperatures
		tft.drawLine(i + 2, 0, i + 2, w, Color::Green);
		tft.drawLine(i + 1, 0, i + 1, w, Color::Black);
		tft.setPixel(i, mid, i & 1 ? Color::Gray70 : Color::Black);
		int q = 0;
        tft.setCursor(0, mid - 4);
        tft.print(midTemp);

		for (int z = scale / 10.0; z < mid; z += scale / 10.0) {
			q++;
			color_t color = Color::Gray20;

			if (q % 10 == 0) {
				color = Color::Gray70;
                tft.setCursor(0, mid - z - 4);
                tft.print(midTemp + (q/10));
                tft.setCursor(0, mid + z - 4);
                tft.print(midTemp - (q/10));
			}

			tft.setPixel(i, mid - z, i & 1 ? color : Color::Black);
			tft.setPixel(i, mid + z, i & 1 ? color : Color::Black);
		}

        // Calculate the resistance of the thermistor
		float Rx = (R2 * R3 + R3 * (R1 + R2) * now / Ve) / (R1 - (R1 + R2) * now / Ve);

        // Convert the resistance into degrees C
		float steinhart = Rx / thermistorNormal;     // (R/Ro)
		steinhart = log(steinhart);                  // ln(R/Ro)
		steinhart /= thermistorBeta;                   // 1/B * ln(R/Ro)
		steinhart += 1.0 / (thermistorAmbient + 273.15); // + (1/To)
		steinhart = 1.0 / steinhart;                 // Invert
		steinhart -= 273.15;
		float gval = steinhart - midTemp;

        // If this is the first ever pass then the "last" value can be the current value. 
        // Just prevents a messy vertical line on the left of the screen at the start.
		if (last < -999000) {
			last = gval;
		}

        // Draw the current graph line.
		tft.drawLine(i, mid - (last * scale), i + 1, mid - (gval * scale), Color::Red);

        // Display the raw data on the screen
		tft.setCursor(0, 0);
		tft.print(now, 8);
		tft.print(" Volts, ");
		tft.print(Rx);
		tft.print(" Ohms, ");
		tft.print(steinhart, 8);
		tft.print(" C   ");

        // Remember this value as the last one.
		last = gval;
	}
}
