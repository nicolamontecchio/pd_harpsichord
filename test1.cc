#include <iostream>
#include <portaudio.h>
#include <stdio.h>
#include <strings.h>
#include <cstdlib>
#include <z_libpd.h>
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
  libpd_process_float(1, (float *) inputBuffer, (float *) outputBuffer);
  return 0;
}


#define SAMPLE_RATE (44100)

void pdprint(const char *s) {
  printf("%s", s);
}


int main(int argc, char **argv)
{

  libpd_set_printhook(pdprint);
  // libpd_set_messagehook(pdmessage); //NOPE
  libpd_init();

  int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
  if(init_err != 0)
  {
    printf("audio init failed with code %d\n", init_err);
    return 1;
  }

  // try to open the test patch
  void *patch = libpd_openfile("libpdtestpatch.pd", ".");
  printf("patch file opened; handle: %d\n", patch);
  libpd_bang("fufi");

  int pd_tick_size = libpd_blocksize();

  printf("PD tick size: %d\n", pd_tick_size);

  // alsa stuff
  int deviceNum = atoi(argv[1]);
  Pa_Initialize();


  PaStreamParameters outputParameters;
  bzero( &outputParameters, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
  outputParameters.channelCount = 2;
  outputParameters.device = deviceNum;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceNum)->defaultLowOutputLatency ;
  outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

  PaStream *stream;
  paTestData data;
  Pa_OpenStream( &stream, NULL, &outputParameters, SAMPLE_RATE, pd_tick_size,  paNoFlag, patestCallback, &data);
  Pa_StartStream( stream );


  Pa_Sleep(3*1000);

  Pa_StopStream( stream );
  Pa_CloseStream( stream );



  libpd_closefile(patch);





}



//   err = Pa_Terminate();
//   if( err != paNoError )
//   {
//     printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
//     return 1;
//   }
