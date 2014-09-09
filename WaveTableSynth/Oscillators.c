//
//  Oscillators.c
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "SoundFiles.h"
#include "Oscillators.h"

// Create a sine wavetable
void createSineWavetable(float * buffer, int sampCnt, float periods)
{
    int k = 0;
    float amplitude = 1;
    float coeff = 2*M_PI/sampCnt;
    
    for (k = 0 ; k <= sampCnt ; k++)
    {
        buffer[k] = amplitude * sinf(periods * k * coeff);
    }
    
    k++;
    buffer[k] = 0;
    
}

// Simple interpolating wavetable oscillator
void wavetableOsc(float * buffer, int bufferSize, float * wavetable, int wavetableSize, float freq, float amp, float dur)
{
    int numSamps = dur*SAMPLE_RATE;
    
    float baseIncr = wavetableSize/44100.0;
    double phaseIncr = baseIncr*freq;
    double indexRem, i = 0.0;
    float watchMe, w1, w2 = 0.0;
    int n, indexInt = 0;
    
    for (n=0; n<numSamps; n++) {
        indexInt = floor(i);
        indexRem = i-indexInt;
        
        w1 = wavetable[indexInt];
        w2 = wavetable[indexInt+1];
        
        watchMe = w1 + ((w2-w1)*indexRem);
        buffer[n] = watchMe*amp;
        
        // manage phasor
        if (indexInt<wavetableSize) {i += phaseIncr;} else {i -= wavetableSize;}
    }
}

// Simple interpolating wavetable oscillator
void wavetableOscBuf(float * buffer, int bufferSize, float * wavetable, int wavetableSize, float freq, float amp, float dur)
{
    int numSamps = dur*SAMPLE_RATE;
    
    float baseIncr = wavetableSize/44100.0;
    double phaseIncr = baseIncr*freq;
    double indexRem, i = 0.0;
    float watchMe, w1, w2 = 0.0;
    int n, indexInt = 0;
    
    for (n=0; n<numSamps; n++) {
        indexInt = floor(i);
        indexRem = i-indexInt;
        
        w1 = wavetable[indexInt];
        w2 = wavetable[indexInt+1];
        
        watchMe = w1 + ((w2-w1)*indexRem);
        buffer[n] = watchMe*amp;
        
        // manage phasor
        if (indexInt<wavetableSize) {i += phaseIncr;} else {i -= wavetableSize;}
    }
}

// Simple/Naive - fill a buffer with a sine

void fillBufferWithSine(float * buffer, float freq)
{
    int k = 0;
    float amplitude = 0.5;
    float coeff = 2*M_PI/SAMPLE_RATE;
    
    for (k = 0 ; k < SOUND_LENGTH ; k++)
     {
     buffer[k] = amplitude * sinf(freq * k * coeff);
     }
}



