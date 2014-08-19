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

// Simple/Naive - fill a buffer with a sine


void fillBufferWithSine(float * buffer, float freq)
{
    int k = 0;
    float amplitude = 0.5;
    float coeff = 2*M_PI/SAMPLE_RATE;
    
    for (k = 0 ; k < SOUND_LENGTH ; k++)
     {
     buffer[k] = amplitude * sinf (freq * 2.0f * k * coeff) ;
     }
}



