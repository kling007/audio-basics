//
//  Oscillators.h
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#ifndef WaveTableSynth_Oscillators_h
#define WaveTableSynth_Oscillators_h

#include "Signal.h"

// create wavetables

void createSineWavetable(float * buffer, int sampCnt, float periods);

// wavetable oscs

void wavetableOsc(float * buffer, int bufferSize, float * wavetable, int wavetableSize, float freq, float amp, float dur);

// wavetable osc that has persistent phase

void wavetableOscBuf(float * buffer, int bufferSize, float * wavetable, int wavetableSize, float freq, float amp, float dur);

// misc functions
void fillBufferWithSine(float * buffer, float freq);

#endif
