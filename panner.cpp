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

#include <Arduino.h>
#include "panner.h"
#include "utility/dspinst.h"

void Panner::update(void){
  audio_block_t *in = receiveReadOnly(0);
  audio_block_t *out[8]; // = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  
  if(in){
    audio_block_t *pan = receiveReadOnly(1);
    if(pan){
      for(int och = 0; och < 8; ++och){
        out[och] = allocate();
        if (out[och]) {
          memset(out[och]->data, 0, AUDIO_BLOCK_SAMPLES*2);
        }else{
          Serial.print("output buffer not allocated "); Serial.println(och);
        }
      }
      for(int i = 0; i < AUDIO_BLOCK_SAMPLES; ++i){
        int32_t p = pan->data[i];
        if(p < 0){
          p = 32768 + p;
        }
        int a = p >> 12;
        int b = (a + 1) % 8;
        int32_t p2 = (p % 4096);
        int32_t p1 = (4096 - p2);
        p2 = sqrt_pan[p2];
        p1 = sqrt_pan[p1];
        if(out[a]){
          int32_t val1 = signed_multiply_32x16b(p1, in->data[i]);
          out[a]->data[i] = signed_saturate_rshift(val1, 16, 0);
        }
        if(out[b]){
          int32_t val2 = signed_multiply_32x16b(p2, in->data[i]);
          out[b]->data[i] = signed_saturate_rshift(val2, 16, 0);
        }
      }
      for(int och = 0; och < 8; ++och){
        if(out[och]){
          transmit(out[och], och);
          release(out[och]);
        }
      }
      release(pan);
    }
    release(in);
  }
}
