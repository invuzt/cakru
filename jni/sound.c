#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <stdint.h>
#include "sound.h"

SLObjectItf engineObj = NULL; SLEngineItf engineItf;
SLObjectItf outputMixObj = NULL; SLObjectItf playerObj = NULL;
SLPlayItf playerPlay; SLBufferQueueItf playerBufferQueue;

// Data suara "BEEP" (1000 sample, frekuensi kotak)
int16_t beep_buffer[4000]; 

void init_sound() {
    // 1. Create Engine
    slCreateEngine(&engineObj, 0, NULL, 0, NULL, NULL);
    (*engineObj)->Realize(engineObj, SL_BOOLEAN_FALSE);
    (*engineObj)->GetInterface(engineObj, SL_IID_ENGINE, &engineItf);
    
    // 2. Create Output Mix
    (*engineItf)->CreateOutputMix(engineItf, &outputMixObj, 0, NULL, NULL);
    (*outputMixObj)->Realize(outputMixObj, SL_BOOLEAN_FALSE);

    // 3. Setup Audio Source (Buffer Queue) & Sink
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, SL_SAMPLINGRATE_44_1,
        SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
        SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioSrc = {&loc_bufq, &format_pcm};

    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObj};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // 4. Create Player
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    (*engineItf)->CreateAudioPlayer(engineItf, &playerObj, &audioSrc, &audioSnk, 1, ids, req);
    (*playerObj)->Realize(playerObj, SL_BOOLEAN_FALSE);
    (*playerObj)->GetInterface(playerObj, SL_IID_PLAY, &playerPlay);
    (*playerObj)->GetInterface(playerObj, SL_IID_BUFFERQUEUE, &playerBufferQueue);
    
    (*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING);

    // 5. Generate Suara Beep (Gelombang Kotak/Square Wave)
    for(int i=0; i<4000; i++) {
        beep_buffer[i] = (i % 100 < 50) ? 5000 : -5000; // Frekuensi rendah (Bass)
    }
}

void play_crash_sound() {
    if (playerBufferQueue) {
        // Masukkan data suara ke antrean speaker
        (*playerBufferQueue)->Clear(playerBufferQueue);
        (*playerBufferQueue)->Enqueue(playerBufferQueue, beep_buffer, sizeof(beep_buffer));
    }
}
