#include <iostream>
#include "portaudio.h"
#include <stdio.h>
#include <strings.h>
using namespace std;


typedef struct
{
  float left_phase;
  float right_phase;
} paTestData;


/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
			   unsigned long framesPerBuffer,
			   const PaStreamCallbackTimeInfo* timeInfo,
			   PaStreamCallbackFlags statusFlags,
			   void *userData )
{
/* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData*)userData;
  float *out = (float*)outputBuffer;
  unsigned int i;
  (void) inputBuffer; /* Prevent unused variable warning. */
  for( i=0; i<framesPerBuffer; i++ )
  {
    *out++ = data->left_phase; /* left */
    *out++ = data->right_phase; /* right */
/* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
    data->left_phase += 0.01f;
/* When signal reaches top, drop back down. */
    if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
/* higher pitch so we can distinguish left and right. */
    data->right_phase += 0.03f;
    if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
  }
  return 0;
}


#define SAMPLE_RATE (44100)

int main(int argc, char **argv)
{

  int err;

  static paTestData data;

  err = Pa_Initialize();
  if( err != paNoError )
  {
    printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    return 1;
  }

  int numDevices;
  numDevices = Pa_GetDeviceCount();
  if( numDevices < 0 )
  {
    printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
    err = numDevices;
  }
  printf("found %d devices\n", numDevices);

  for(int i=0; i<numDevices; i++ )
  {
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo( i );
    printf("device %d: %s\n", i, deviceInfo->name);
  }

  PaStreamParameters outputParameters;
  bzero( &outputParameters, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
  outputParameters.channelCount = 2;
  outputParameters.device = 2;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(2)->defaultLowOutputLatency ;
  outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

  PaStream *stream;
  // PaError err;
/* Open an audio I/O stream. */
  err = Pa_OpenStream( &stream, NULL, &outputParameters, SAMPLE_RATE, 256,  paNoFlag, patestCallback, &data);

  err = Pa_StartStream( stream );

  Pa_Sleep(10*1000);

  err = Pa_StopStream( stream );

  err = Pa_CloseStream( stream );

  err = Pa_Terminate();
  if( err != paNoError )
  {
    printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    return 1;
  }
}
