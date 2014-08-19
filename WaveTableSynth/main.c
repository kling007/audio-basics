//
//  main.c
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "SoundFiles.h"
#include "Oscillators.h"


int main(int argc, const char * argv[])
{
    const char * fname ="grainTest.aif";
    SNDFILE	* file;
    SF_INFO	sfinfo ;
    
    // Allocate memory for the audio buffer
    float* audioBuffer = (float*)malloc(sizeof(float)*SOUND_LENGTH);
    
    // setup audiofile spec
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    
    // print info
    printf("Welcome to the whole grains bakery!\n");
    printf("Creating the sound loaf.\n");
    
    // open sfile for writing
    if (create_file (fname, &file, sfinfo))
    {
        printf("Failed to open soundfile. Exiting...\n");
        return 1;
    }
    
    // Read/create events for the synth
    // later
    
    // init Ugens - create wavetables and envelope shapes
    
    // produce audio in buffer
    fillBufferWithSine(audioBuffer, 440.0);
    
    // write buffer to disk
    if (write_file(audioBuffer, file, sfinfo))
        {
            printf("Failed to write soundfile. Exiting\n");
            return 1;
        }
    
    // close file
    close_file(file);
    
    // error reports
    
    // free the sound buffer
    free(audioBuffer);
    
    return 0;
}

