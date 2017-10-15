#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <z_libpd.h>
#include <portaudio.h>
#include <alsa/asoundlib.h>



// needed by pd stuff, otherwise linking issues (?)
void stoptrigger_setup(void);
void sampleplayer_tilde_setup(void);



const int MIDI_BUFFER_LEN = 1000;
const int MIDINOTEOFF     = 0x80;
const int MIDINOTEON      = 0x90;
const int SAMPLE_RATE     = 44100;


int stop = 0;

void sighandler(int dum)
{
  printf("setting stop flag\n");
  fflush(stdout);
  stop=1;
}


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

  signal(SIGINT, sighandler);

  libpd_set_printhook(pdprint);
  libpd_init();
  mraagpioin_setup();
  mraagpioout_setup();
  stoptrigger_setup();
  sampleplayer_tilde_setup();

  int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
  if(init_err != 0)
  {
    printf("audio init failed with code %d\n", init_err);
    return 1;
  }

  void *patch = libpd_openfile("blanchet1720.pd", ".");
  printf("patch file opened; handle: %d\n", patch);

  flip_register(0); // turn 8' on by default

  // dsp on (?)
  libpd_start_message(1);
  libpd_add_float(1.0f);
  libpd_finish_message("pd", "dsp");
  printf("pd dsp on\n");
  fflush(stdout);

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

  Pa_OpenStream( &audio_stream, NULL, &outputParameters, SAMPLE_RATE, libpd_blocksize(),  paNoFlag, patestCallback, NULL);
  Pa_StartStream( audio_stream );

  printf("portaudio stream initialized\n");
  fflush(stdout);

  printf("dsp is on\n");
  fflush(stdout);

  char portname[32];
  sprintf(portname, "hw:%d", midi_device_num);
  snd_ctl_t *ctl;
  snd_ctl_open(&ctl, portname, 0);
  snd_rawmidi_t* midiin = NULL;
  snd_rawmidi_open(&midiin, NULL, portname, SND_RAWMIDI_NONBLOCK);
  char buffer;
  int note_cycle = 0; // in [0,3)
  unsigned int note_message[3];

  do
  {
    int status = snd_rawmidi_read(midiin, &buffer, 1);
    if (status == -11) {}
    else if (status < 0)
    {
      printf("Problem reading MIDI input [%d]: %s\n", status, snd_strerror(status));
      break;
    }
    else
    {
      if ((unsigned char)buffer>= 0x80) // command byte
      {
      	note_cycle = 0;
	note_message[note_cycle++] = buffer;
      }
      else
      {
	note_message[note_cycle++] = buffer;
      	if(note_cycle == 3)
      	{
	  unsigned int msg_type = note_message[0] & 0xff;
	  /* printf("msg_type: %u\n", msg_type); */
	  if(msg_type == MIDINOTEON)
	  {
	    /* printf("libpd_noteon\n"); */
	    libpd_noteon(1, note_message[1], note_message[2]);
	  }
	  else if(msg_type == MIDINOTEOFF)
	  {
	    /* printf("libpd_noteoff\n"); */
	    libpd_noteon(1, note_message[1], 0);
	  }
	  printf("midi message: %u %u %u\n", note_message[0], note_message[1], note_message[2]);
      	  note_cycle = 0;
      	}
      }
    }
    Pa_Sleep(2);
  } while(!stop);

  Pa_StopStream( audio_stream );
  Pa_CloseStream( audio_stream );
  Pa_Terminate();

  snd_rawmidi_close(midiin);
  snd_ctl_close(ctl);
  mraa_deinit();

  libpd_closefile(patch);
}
