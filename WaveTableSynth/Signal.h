//
//  Signal.h
//  WaveTableSynth
//
//  Created by Garry Kling on 8/18/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#ifndef WaveTableSynth_Signal_h
#define WaveTableSynth_Signal_h

#include "sndfile.h"

struct Signal
{
    double sampRate;
    double sampInterval;
    float * sigBuf;
    int chanCnt;
    int sampCnt;
    int frameCnt;
    int * infoTags;
    SNDFILE * sigFile;
};

typedef Signal Signal;


#endif
