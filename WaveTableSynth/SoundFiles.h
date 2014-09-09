//
//  SoundFiles.h
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#ifndef WaveTableSynth_SoundFiles_h
#define WaveTableSynth_SoundFiles_h
#include "sndfile.h"

#define		BUFFER_LEN		1024
#define     SAMPLE_RATE     44100
#define     SOUND_LENGTH    88200


#ifndef		M_PI
#define		M_PI		3.14159265358979323846264338
#endif

// Soundfile Utility Functions
int create_file (const char * fname, SNDFILE ** file, SF_INFO * sfinfo);
int write_file (float * buffer, SNDFILE * file, SF_INFO * sfinfo);
int close_file (SNDFILE * file);
#endif
