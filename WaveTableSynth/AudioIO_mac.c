//
//  AudioIO_mac.c
//  WaveTableSynth
//
//  Created by Garry Kling on 8/28/14.
//  Copyright (c) 2014 Garry Kling. All rights reserved.
//

// C
#include <stdio.h>
#include "AudioIO_mac.h"
//#include <CoreAudio/CoreAudioTypes.h>

// the base callback for AudioQueues

static void SoundBufferHandler(void * infoHdl, AudioQueueRef inQ, AudioQueueBufferRef inBuffer)
{
	SoundIO_Info * theInfo = (SoundIO_Info *)infoHdl;
	if (!theInfo->mIsRunning) return;
    
	UInt32 bytesWritten = 0;
    OSStatus result = 0;
    
    // this is where we need to gather data from our oscillator
    // needs to be packed into inBuffer
    // main will prob have to call osc and then send buffer to play sound, this will not be real time
    // until we have a ugen model for the oscs
    // the func will probably have to deliver a sentinel value to trigger the closing of the audio queue when done
    
	//OSStatus result = AudioFileReadPackets(gInfo->mAudioFile, false, &numBytes, gInfo->mPacketDescs, gInfo->mCurrentPacket, &nPackets, inBuffer->mAudioData);
	
    mixer(inBuffer->mAudioData, theInfo->audioBufPtr, inBuffer->mAudioDataBytesCapacity, &bytesWritten);
	
    inBuffer->mAudioDataByteSize = bytesWritten;
    
    // push the inBuffer into the audio Queue or shut off playback
    
	if (bytesWritten > 0) {
		AudioQueueEnqueueBuffer(inQ, inBuffer, 0, theInfo->mPacketDescs);
	} else {
		result = AudioQueueStop(theInfo->mQueue, false);
		if (result) {
			printf ("AudioQueueStop(false) failed: %d", (int)result);
            theInfo->mIsRunning = false;
			return;
		}
        // reading nPackets == 0 is our EOF condition
		theInfo->mIsRunning = false;
	}
}

// Initialize the AudioQueue and get it ready

int initAudioOutput(float * outputBuf)
{
    SoundIO_Info lInfo;   // Should this be static? owned by main()?
    OSStatus err = 0;
    Float32 gain = 0.5;
    AudioStreamBasicDescription tempAudioDataFormat = {0};

    // Custom AudioStreamBasicDescription (ASBD) to populate the lInfo->mDataFormat
    
    tempAudioDataFormat.mSampleRate = 44100;
    tempAudioDataFormat.mFormatID = kAudioFormatLinearPCM;
    tempAudioDataFormat.mFormatFlags = kAudioFormatFlagIsPacked | kAudioFormatFlagIsFloat;
    tempAudioDataFormat.mBytesPerPacket = 4;
    tempAudioDataFormat.mFramesPerPacket = 1;
    tempAudioDataFormat.mBytesPerFrame = NUM_CHANS * sizeof(float);
    tempAudioDataFormat.mChannelsPerFrame = NUM_CHANS;
    tempAudioDataFormat.mBitsPerChannel = 8 * sizeof(float);
    tempAudioDataFormat.mReserved = 0;
    
    lInfo.mDataFormat = tempAudioDataFormat;
    lInfo.audioBufPtr = outputBuf;
    
    // need to create a custom AudioStreamPacketDescription (ASPD) to populate the lInfo->mPacketDescs
    // For PCM, this is set to NULL, don't think I'll support VBR/compressed formats
    lInfo.mPacketDescs = NULL;
    
    // Create the audio queue
    
    err = AudioQueueNewOutput(&lInfo.mDataFormat, SoundBufferHandler, &lInfo,
                        CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &lInfo.mQueue);
    // see if we are OK
    if (err) {
        handleCoreAudioErrorCode(err);
        return 1;
    }
    
    // Set buffer size, etc. so callback will write the correct amount of data
    lInfo.bufferByteSize = IO_BUFFER_LEN*sizeof(float)*NUM_CHANS;
    
    // Alloc and prime the queue buffers
    
    lInfo.mCurrentPacket = 0;
    
    for (int i = 0; i < kNumberBuffers; ++i) {
        err = AudioQueueAllocateBuffer (lInfo.mQueue, lInfo.bufferByteSize, &lInfo.mBuffers[i]);
        if (err) {
            handleCoreAudioErrorCode(err);
            // should really dealloc any buffers here, but we currently die from this anyway
            return 1;
        }
        // Hit Play
        lInfo.mIsRunning = true;
        
        // Prime the queue by directly calling my callback - once I have it a bit more hammered out
        
        SoundBufferHandler(&lInfo, lInfo.mQueue, lInfo.mBuffers[i]);
    }
    
    // Set the volume?
    err = AudioQueueSetParameter(lInfo.mQueue, kAudioQueueParam_Volume, gain);
    if (err) {
        handleCoreAudioErrorCode(err);
        // should really dealloc any buffers here, but we currently die from this anyway
        return 1;
    }
    
    
    err = AudioQueueStart (lInfo.mQueue, NULL);
    if (err) {
        handleCoreAudioErrorCode(err);
        // should really dealloc any buffers here, but we currently die from this anyway
        return 1;
    }
    
    do {
        CFRunLoopRunInMode (kCFRunLoopDefaultMode, 0.01, false);
    } while(lInfo.mIsRunning);
    
    CFRunLoopRunInMode (kCFRunLoopDefaultMode, 1, false);
    
    // Cleanup after the queue stops
    err = AudioQueueDispose (lInfo.mQueue, true);
    if (err) {
        handleCoreAudioErrorCode(err);
        // should really dealloc any buffers here, but we currently die from this anyway
        return 1;
    }
    
    return 0;
}


void handleCoreAudioErrorCode(OSStatus theErr)
{
    switch (theErr) {
        case kAudioQueueErr_InvalidBuffer:
            printf("CoreAudio error: InvalidBuffer\n");
            break;
        case kAudioQueueErr_BufferEmpty:
            printf("CoreAudio error: BufferEmpty\n");
            break;
        case kAudioQueueErr_DisposalPending:
            printf("CoreAudio error: DisposalPending\n");
            break;
        case kAudioQueueErr_InvalidProperty:
            printf("CoreAudio error: InvalidProperty\n");
            break;
        case kAudioQueueErr_InvalidPropertySize:
            printf("CoreAudio error: InvalidPropertySize\n");
            break;
        case kAudioQueueErr_InvalidParameter:
            printf("CoreAudio error: InvalidParameter\n");
            break;
        case kAudioQueueErr_CannotStart:
            printf("CoreAudio error: CannotStart\n");
            break;
        case kAudioQueueErr_InvalidDevice:
            printf("CoreAudio error: InvalidDevice\n");
            break;
        case kAudioQueueErr_BufferInQueue:
            printf("CoreAudio error: BufferInQueue\n");
            break;
        case kAudioQueueErr_InvalidRunState:
            printf("CoreAudio error: InvalidRunState\n");
            break;
        case kAudioQueueErr_InvalidQueueType:
            printf("CoreAudio error: InvalidQueueType\n");
            break;
        case kAudioQueueErr_Permissions:
            printf("CoreAudio error: Permissions error\n");
            break;
        case kAudioQueueErr_InvalidPropertyValue:
            printf("CoreAudio error: InvalidPropertyValue\n");
            break;
        case kAudioQueueErr_PrimeTimedOut:
            printf("CoreAudio error: PrimeTimedOut\n");
            break;
        case kAudioQueueErr_CodecNotFound:
            printf("CoreAudio error: CodecNotFound\n");
            break;
        case kAudioQueueErr_InvalidCodecAccess:
            printf("CoreAudio error: InvalidCodecAccess\n");
            break;
        case kAudioQueueErr_QueueInvalidated:
            printf("CoreAudio error: QueueInvalidated\n");
            break;
        case kAudioQueueErr_TooManyTaps:
            printf("CoreAudio error: TooManyTaps\n");
            break;
        case kAudioQueueErr_InvalidTapContext:
            printf("CoreAudio error: InvalidTapContext\n");
            break;
        case kAudioQueueErr_RecordUnderrun:
            printf("CoreAudio error: RecordUnderrun\n");
            break;
        case kAudioQueueErr_InvalidTapType:
            printf("CoreAudio error: InvalidTapType\n");
            break;
        case kAudioQueueErr_EnqueueDuringReset:
            printf("CoreAudio error: EnqueueDuringReset\n");
            break;
        case kAudioQueueErr_InvalidOfflineMode:
            printf("CoreAudio error: InvalidOfflineMode\n");
            break;
            
        default:
            printf("Unknown CoreAudio error\n");
            break;
    }
}
