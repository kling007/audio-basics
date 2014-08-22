//
//  SoundFiles.c
//  WaveTableSynth
//
//  Created by Garry Kling on 8/17/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#include <stdio.h>
#include "sndfile.h"
#include "SoundFiles.h"

// This function creates a local soundfile for writing

int create_file (const char * fname, SNDFILE ** file, SF_INFO * sfinfo)
{
    
    if (! (*file = sf_open (fname, SFM_WRITE, sfinfo)))
	{
        printf ("Error : Not able to open output file.\n") ;
		return 1 ;
    } ;
    
    return 0;
} /* create_file */

int write_file (float * buffer, SNDFILE * file, SF_INFO * sfinfo)
{
    int numSamps = sfinfo->frames * sfinfo->channels;
    sf_write_float (file, buffer, numSamps);
    puts (sf_strerror (file)) ;
    return 0;
} /* write_file */

int close_file (SNDFILE * file)
{
    sf_close(file);
    return 0;
}

