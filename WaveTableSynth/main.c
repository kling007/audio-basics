//
//  main.c
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Application includes
#include "SoundFiles.h"
#include "Oscillators.h"
#include "Signal.h"

int main(int argc, const char * argv[])
{
    const char * fname ="wavetableOSCTest.aif";
    SNDFILE	* file;
    SF_INFO sfinfo;
    int soundSize = 8192;
    
    // Allocate memory for the audio buffer
    float* audioBuffer = (float*)malloc(sizeof(float)*SOUND_LENGTH);
    float* sineTable = (float*)malloc(sizeof(float)*soundSize+1);
    
    // setup audiofile spec
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    
    // Init Wavetables
    createSineWavetable(sineTable, soundSize, 1.0f);
    
    // produce audio in buffer
    wavetableOsc(audioBuffer, SOUND_LENGTH, sineTable, soundSize, 440, 0.5, 1.5);
    
    // print info
    printf("Welcome to the whole grains bakery!\n");
    printf("Creating the sound loaf.\n");
    
    
    sfinfo.samplerate	= SAMPLE_RATE;
	sfinfo.frames		= soundSize;
	sfinfo.channels		= 1;
	sfinfo.format		= (SF_FORMAT_AIFF | SF_FORMAT_PCM_16) ;
    
    
    // open sfile for writing
    if (create_file (fname, &file, &sfinfo))
    {
        printf("Failed to open soundfile. Exiting...\n");
        return 1;
    }
    
    // Read/create events for the synth
    
    
    // write buffer to disk
    // frames is reset after call to sf_create_file
    sfinfo.frames		= SOUND_LENGTH;
    
    if (write_file(audioBuffer, file, &sfinfo))
        {
            printf("Failed to write soundfile. Exiting\n");
            return 1;
        }
    
    // close file
    close_file(file);
    
    // error reports
    
    // free the sound buffer
    free(audioBuffer);
    free(sineTable);
    
    return 0;
}

