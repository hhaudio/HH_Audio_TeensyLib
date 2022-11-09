/* Audio Library for Teensy 3.X
 * Copyright (c) 2014, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef arbitrary_waveform_h_
#define arbitrary_waveform_h_

#include <Arduino.h>
#include "AudioStream.h"
#include "arm_math.h"

#warning "arbitrary wave!!!"

class ArbitraryWaveform : public AudioStream
{
public:
	ArbitraryWaveform() : AudioStream(0, NULL), magnitude(65536), inc(0), phase(0), offset(0), wavetable(NULL) {}
	// AudioSynthWaveformSine() : AudioStream(0, NULL), magnitude(16384) {}

	void begin( int16_t *data, int32_t len){
		wavetable = data;
		wavelenf = wavelen = len; 
		phase = 0;
	}

	void frequency(float freq) {
		if(wavetable == NULL){
			return;
		}
		inc = wavelenf * freq / float(AUDIO_SAMPLE_RATE_EXACT);
	}

	void amplitude(float n) {	// 0 to 1.0
		if (n < 0) {
			n = 0;
		}
		magnitude = n * 65536.0;
	}

	void set_mag(int32_t mag){
		magnitude = mag;
	}

	int32_t get_mag(){
		return magnitude;
	}

	void normalize(){
		if(wavetable == NULL){
			return;
		}
		float maxf = 0;
		for(int i = 0; i < wavelen; ++i){
			if(abs(wavetable[i] + offset) > maxf){ maxf = abs(wavetable[i] + offset); }
		}
		amplitude(32767.0f / maxf);
		Serial.println(maxf);
		Serial.println(32767.0f / maxf);
		Serial.println(magnitude);
	}

	void byass(){
		if(wavetable == NULL){
			return;
		}
		int max = 0;
		int min = 0;
		for(int i = 0; i < wavelen; ++i){
			if(wavetable[i] > max){ max = wavetable[i]; }
			else if(wavetable[i] < min){ min = wavetable[i]; }
		}

		offset = (min - max) >> 1;

		if(min > 0){ offset -= min; }
		else if(max < 0){ offset += max; }

		Serial.println(offset);
	}

	void normalize_byass(){
		byass();
		normalize();
	}

	virtual void update(void);

private:
	const int16_t *wavetable = NULL;
	uint16_t wavelen;
	int32_t magnitude;
	float wavelenf;
	float phase;
	float inc;
	int32_t offset;
};


#endif
