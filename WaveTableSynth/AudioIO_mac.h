//
//  AudioSetup.h
//  WaveTableSynth
//
//  Created by Garry Kling on 9/8/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

#ifndef WaveTableSynth_AudioIO_mac_h
#define WaveTableSynth_AudioIO_mac_h

#include <AudioToolbox/AudioQueue.h>
#include <AudioToolbox/AudioFile.h>
#include <AudioToolbox/AudioFormat.h>
#include <CoreAudio/CoreAudioTypes.h>

// Functions to manage audio IO on OSX

int initAudioOutput();
static void SoundBufferHandler(void * infoHdl, AudioQueueRef inQ, AudioQueueBufferRef inBuffer);
void handleCoreAudioErrorCode(OSStatus theErr);
void mixer(void * outBuf, float * inBuf, UInt32 numBytes, UInt32 * bytesWritten);


static const int kNumberBuffers = 3;
static UInt32 gIsRunning = 0;

struct SoundIO_Info {
	AudioStreamBasicDescription   mDataFormat;                    //
    AudioQueueRef                 mQueue;                         //
    AudioQueueBufferRef           mBuffers[kNumberBuffers];       //
    UInt32                        bufferByteSize;                 //
    SInt64                        mCurrentPacket;                 //
    UInt32                        mNumPacketsToRead;              //
    AudioStreamPacketDescription  *mPacketDescs;                  //
    bool                          mIsRunning;                     //
	float *                       audioBufPtr;
};

typedef struct SoundIO_Info SoundIO_Info;

// static struct SoundIO_Info * gInfo;

// Useful defines - is this good style?
#define     IO_BUFFER_LEN   1024
#define     NUM_CHANS       1

#endif
