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

// For Core Audio

#include "AudioIO_mac.h"

// For when we shift to a UnitGenerator model of synthesis
//#include "Signal.h"
//#include "AudioSetup.h"


int main(int argc, const char * argv[])
{
    const char * fname ="wavetableOSCTest.aif";
    SNDFILE	* file;
    SF_INFO sfinfo;
    int soundSize = 8192;
    
    // print info
    printf("Welcome to the whole grains bakery!\n");
    printf("Creating the sound loaf.\n");
    
    // Allocate memory for the audio buffer
    float* audioBuffer = (float*)malloc(sizeof(float)*SOUND_LENGTH);
    float* sineTable = (float*)malloc(sizeof(float)*soundSize+1);
    
    // setup audiofile spec
    memset (&sfinfo, 0, sizeof (sfinfo)) ;
    
    // Init Wavetables
    createSineWavetable(sineTable, soundSize, 1.0);
    
    // (set up ugen graph)
    
    // register it so it will play when mixer() is called
    
    // produce audio in buffer
    wavetableOsc(audioBuffer, SOUND_LENGTH, sineTable, soundSize, 880, 0.5, 5);
    
    // setup audio playback - hard coded at the moment until I understand what options I will need to be able to adjust
    
    if (initAudioOutput()) {
        printf("Failed to setup audio. Exiting...\n");
        // retrieve and log/printf CoreAudio error code
        return 1;
    }
    
    // play the audio
    
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


// nothing yet, will eventually be the internal UGen graph driver
void mixer(void * outBuf, float * inBuf, UInt32 numBytes, UInt32 * bytesWritten)
{
    static int totalSamplesWritten = 0;
    UInt32 bytesThisTime = 0;
    UInt32 byteSize = sizeof(float);
    float * localBuf = (float*)outBuf;
    int i = 0;
    
    // if our buffer is empty, set bytesWritten to 0 and return
    if (totalSamplesWritten>=SOUND_LENGTH) {
        *bytesWritten = 0;
        return;
    }
    
    while (bytesThisTime<numBytes && totalSamplesWritten<SOUND_LENGTH) {
        // not sure I'm using the void * properly
        localBuf[i] = inBuf[totalSamplesWritten];
        i++;
        totalSamplesWritten++;
        bytesThisTime = i*byteSize;
    }
    
    *bytesWritten = bytesThisTime;

}