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

#ifndef mixer_xch_h_
#define mixer_xch_h_

#include "Arduino.h"
#include "AudioStream.h"

#define MIXER_XCH_MAXCHANNELS 32

class Mixer_XCH : public AudioStream
{
public:
    Mixer_XCH( void ) : AudioStream(MIXER_XCH_MAXCHANNELS, inputQueueArray) {}
    virtual void update(void);
    void gain(unsigned int channel, float gain) {
        if (channel >= MIXER_XCH_MAXCHANNELS) return;
        if (gain > 32767.0f) gain = 32767.0f;
        else if (gain < -32767.0f) gain = -32767.0f;
        multiplier[channel] = gain * 65536.0f; // TODO: proper roundoff?
    }

    int32_t multiplier[MIXER_XCH_MAXCHANNELS];
  
private:
	audio_block_t *inputQueueArray[MIXER_XCH_MAXCHANNELS];



};

#endif
