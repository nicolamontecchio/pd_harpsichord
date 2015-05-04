#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <z_libpd.h>
#include <portaudio.h>
#ifdef EDISON
#include <alsa/asoundlib.h>
#else
#include <portmidi.h>
#endif


const int MIDI_BUFFER_LEN = 1000;
const int MIDINOTEOFF     = 0x80;
const int MIDINOTEON      = 0x90;
const int SAMPLE_RATE     = 44100;

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


void pdprint(const char *s) {
  printf("%s", s);
}


void flip_register(int reg) {
  libpd_start_message(10);
  libpd_add_float(reg);
  libpd_finish_message("stopcontrol_set", "stoptoggle");
}


int main(int argc, char **argv)
{
  int audio_device_num = atoi(argv[1]);
  int midi_device_num = atoi(argv[2]);
  PaStream *audio_stream;

  libpd_set_printhook(pdprint);
  libpd_init();

  int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
  if(init_err != 0)
  {
    printf("audio init failed with code %d\n", init_err);
    return 1;
  }

  // dsp on (?)
  libpd_start_message(1);
  libpd_add_float(1.0f);
  libpd_finish_message("pd", "dsp");

  void *patch = libpd_openfile("blanchet1720.pd", ".");
  printf("patch file opened; handle: %d\n", patch);

  flip_register(0); // turn 8' on by default

  int pd_tick_size = libpd_blocksize();

  Pa_Initialize();
  const PaDeviceInfo    *audio_device_info = Pa_GetDeviceInfo(audio_device_num);
  printf("using audio device [%d]: %s\n", audio_device_num, audio_device_info->name);


  PaStreamParameters outputParameters;
  bzero( &outputParameters, sizeof( outputParameters ) );
  outputParameters.channelCount = 2;
  outputParameters.device = audio_device_num;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(audio_device_num)->defaultLowOutputLatency ;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  Pa_OpenStream( &audio_stream, NULL, &outputParameters, SAMPLE_RATE, pd_tick_size,  paNoFlag, patestCallback, NULL);
  Pa_StartStream( audio_stream );

  /* PortMidiStream *midi_stream; */
  /* PmEvent *midi_event_buffer = (PmEvent*) malloc(sizeof(PmEvent) * MIDI_BUFFER_LEN); */
  /* Pm_Initialize(); */
  /* const PmDeviceInfo    *midi_device_info  = Pm_GetDeviceInfo(midi_device_num); */
  /* printf("using midi device  [%d]: %s\n", midi_device_num, midi_device_info->name); */
  /* Pm_OpenInput(&midi_stream, midi_device_num, NULL, MIDI_BUFFER_LEN, NULL, NULL); */

  do
  {
    /* if(Pm_Poll(midi_stream)) */
    /* { */
    /*   int n_midi_ev_read = Pm_Read(midi_stream, midi_event_buffer, MIDI_BUFFER_LEN); */
    /*   for(int i = 0; i < n_midi_ev_read; i++) */
    /*   { */
    /* 	PmEvent msg = midi_event_buffer[i]; */
    /* 	int status = Pm_MessageStatus(msg.message); */
    /* 	int data1  = Pm_MessageData1(msg.message); */
    /* 	int data2  = Pm_MessageData2(msg.message); */
    /* 	int data3  = ((msg.message >> 24) & 0xff); */
    /* 	int msgtype = ((status & 0xf0) == 0xf0 ? */
    /* 		       status : (status & 0xf0)); */
    /* 	switch(msgtype) */
    /* 	{ */
    /* 	case MIDINOTEON: */
    /* 	  libpd_noteon(1, data1, data2); */
    /* 	  break; */
    /* 	case MIDINOTEOFF: */
    /* 	  libpd_noteon(1, data1, 0); */
    /* 	  break; */
    /* 	default: */
    /* 	  printf("wtf\n"); */
    /* 	} */
    /*   } */
    /* } */
    Pa_Sleep(2);
  } while(1);

  Pa_StopStream( audio_stream );
  Pa_CloseStream( audio_stream );
  Pa_Terminate();

  /* Pm_Close(midi_stream); */
  /* Pm_Terminate(); */
  /* free(midi_event_buffer); */

  libpd_closefile(patch);
}
