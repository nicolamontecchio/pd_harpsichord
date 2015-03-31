#include <stdio.h>
#include <strings.h>
#include <cstdlib>
#include <z_libpd.h>
#include <portaudio.h>
#include <portmidi.h>

static int patestCallback( const void *inputBuffer, void *outputBuffer,
			   unsigned long framesPerBuffer,
			   const PaStreamCallbackTimeInfo* timeInfo,
			   PaStreamCallbackFlags statusFlags,
			   void *userData )
{
  float *out = (float*)outputBuffer;
  libpd_process_float(1, NULL, out);
  return 0;
}


#define SAMPLE_RATE (44100)

void pdprint(const char *s) {
  printf("%s", s);
}


int main(int argc, char **argv)
{
  int audio_device_num = atoi(argv[1]);
  int midi_device_num = atoi(argv[2]);

  libpd_set_printhook(pdprint);
  // libpd_set_messagehook(pdmessage); //NOPE
  libpd_init();

  int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
  if(init_err != 0)
  {
    printf("audio init failed with code %d\n", init_err);
    return 1;
  }

  // dsp on (?)
  libpd_start_message(1); // one entry in list
  libpd_add_float(1.0f);
  libpd_finish_message("pd", "dsp");

  // try to open the test patch
  void *patch = libpd_openfile("libpdtestpatch.pd", ".");
  printf("patch file opened; handle: %d\n", patch);
  libpd_bang("fufi");

  int pd_tick_size = libpd_blocksize();

  printf("PD tick size: %d\n", pd_tick_size);

  Pm_Initialize();
  Pa_Initialize();
  const PmDeviceInfo    *midi_device_info  = Pm_GetDeviceInfo(midi_device_num);
  const PaDeviceInfo    *audio_device_info = Pa_GetDeviceInfo(audio_device_num);

  printf("using audio device [%d]: %s\n", audio_device_num, audio_device_info->name);
  printf("using midi device  [%d]: %s\n", midi_device_num, midi_device_info->name);

  PaStreamParameters outputParameters;
  bzero( &outputParameters, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
  outputParameters.channelCount = 2;
  outputParameters.device = audio_device_num;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(audio_device_num)->defaultLowOutputLatency ;
  outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

  PaStream *stream;
  Pa_OpenStream( &stream, NULL, &outputParameters, SAMPLE_RATE, pd_tick_size,  paNoFlag, patestCallback, NULL);
  Pa_StartStream( stream );

  Pa_Sleep(5*1000);

  Pa_StopStream( stream );
  Pa_CloseStream( stream );
  Pa_Terminate();

  Pm_Terminate();

  libpd_closefile(patch);

}
