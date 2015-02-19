//
//  main.cpp
//  example
//
//  Created by Guillot Pierre on 28/01/2015.
//  Copyright (c) 2015 Kiwi. All rights reserved.
//

#include <iostream>
#include <stdlib.h>

#include "../../../Implementation/DspPortAudio.h"

using namespace Kiwi;


int main(int argc, const char * argv[])
{
    shared_ptr<PortAudioDeviceManager> device = make_shared<PortAudioDeviceManager>();
    
    if(device)
    {
        sDspContext ctxt = make_shared<DspContext>(device); // Instance
        if(ctxt)
        {
            try
            {
                ctxt->start();
            }
            catch (exception& e)
            {
                cout << e.what() << endl;
            }
            sDspChain cha = make_shared<DspChain>(ctxt); // Patcher
            if(cha)
            {
                ctxt->add(cha);
                sDspNode pSig = make_shared<DspSig>(cha, 1.1);
                sDspNode pAdd = make_shared<DspPlus<Scalar>>(cha, 1.2);
                sDspNode pvAdd = make_shared<DspPlus<Vector>>(cha);
                sDspNode pvAdd2 = make_shared<DspPlus<Vector>>(cha);
                sDspNode pNoise = make_shared<DspNoise>(cha);
                vector<ulong>  outs = {(ulong)1, (ulong)2};
                sDspNode pDac = make_shared<DspDac>(cha, outs);
                
                sDspLink cnect1 = make_shared<DspLink>(cha, pSig, 0, pAdd, 0);
                sDspLink cnect2 = make_shared<DspLink>(cha, pAdd, 0, pvAdd, 0);
                sDspLink cnect3 = make_shared<DspLink>(cha, pSig, 0, pvAdd, 1);
                sDspLink cnect4 = make_shared<DspLink>(cha, pvAdd, 0, pvAdd2, 0);
                sDspLink cnect5 = make_shared<DspLink>(cha, pNoise, 0, pvAdd2, 0);
                sDspLink cnect6 = make_shared<DspLink>(cha, pNoise, 0, pDac, 0);
                sDspLink cnect7 = make_shared<DspLink>(cha, pNoise, 0, pDac, 1);
                
                cha->add(pSig);
                cha->add(pAdd);
                cha->add(pvAdd);
                cha->add(pvAdd2);
                cha->add(pNoise);
                cha->add(pDac);
                
                cha->add(cnect4);
                cha->add(cnect1);
                cha->add(cnect2);
                cha->add(cnect3);
                cha->add(cnect5);
                cha->add(cnect6);
                
                try
                {
                    cha->start();
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                }
                
                
                Pa_Sleep(1000);
                cha->add(cnect7);
                Pa_Sleep(1000);
                
                try
                {
                    cha->start();
                }
                catch (exception& e)
                {
                    cout << e.what() << endl;
                }
            }
        }
    }
    device.reset();
    

    return 0;
}


/*
#define OUTPUT_DEVICE Pa_GetDefaultOutputDevice()
#define INPUT_DEVICE Pa_GetDefaultInputDevice()

#define NUM_SECONDS   (3)
#define NUM_LOOPS     (4)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (1)

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
}
paTestData;


static int patestCallback( const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData )
{
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    const float *in = (float*)inputBuffer;
    unsigned long i;
    //cout << in << endl;
    //cout << out << endl;
    //cout << framesPerBuffer << endl;
    (void) timeInfo;
    (void) statusFlags;
    (void) inputBuffer;
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->sine[data->left_phase];
        *out++ = data->sine[data->right_phase];
        data->left_phase += 1;
        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
        data->right_phase += 3;         if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
    }
    
    return paContinue;
}


int main(void);
int main(void)
{
    const PaStreamInfo* infoss;
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
    PaStream *stream = nullptr;
    PaError err;
    paTestData data;
    int i;
    
    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d\n", SAMPLE_RATE, FRAMES_PER_BUFFER);
    

    for( i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = data.right_phase = 0;
    
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    
    outputParameters.device = OUTPUT_DEVICE;
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        goto error;
    }
    outputParameters.channelCount = 2;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = 0;//64. / 44100.;//Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    inputParameters.device = INPUT_DEVICE;
    inputParameters.channelCount = 2;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = 0;//64. / 44100.;//Pa_GetDeviceInfo( outputParameters.device )->defaultLowInputLatency;
    
    inputParameters.hostApiSpecificStreamInfo = NULL;
    cout << "Stream " << stream << endl;
    err = Pa_OpenStream(
                        &stream,
                        &inputParameters,
                        &outputParameters,
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff,
                        patestCallback,
                        &data );
    if( err != paNoError ) goto error;
    
    cout << "Stream " << stream << endl;
    infoss = Pa_GetStreamInfo(stream);
    cout << "input time " << infoss->inputLatency * 44100<< endl;
    cout << "output time " << infoss->outputLatency  * 44100<< endl;
    
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
    
    for( i=0; i<1; i++ )
    {
        data.left_phase = data.right_phase = 0;
        
        
        
        printf("Play for %d seconds.\n", NUM_SECONDS );
        Pa_Sleep( NUM_SECONDS * 1000 );

    }
    
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;
    
    cout << "Stream " << stream << endl;
    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto error;
    
    cout << "Stream " << stream << endl;
    Pa_Terminate();
    printf("Test finished.\n");
    
    return err;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}
*/
