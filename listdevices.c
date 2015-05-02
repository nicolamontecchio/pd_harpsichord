#include <stdio.h>
#include <portaudio.h>
#ifdef EDISON

#else
#include <portmidi.h>
#endif

int main(int argc, char **argv)
{

  int n, num_audio_devices, num_midi_devices;

  /* printf("list of audio devices:\n"); */
  /* Pa_Initialize(); */
  /* num_audio_devices = Pa_GetDeviceCount(); */
  /* for(n = 0; n < num_audio_devices; n++) */
  /* { */
  /*   const PaDeviceInfo *di = Pa_GetDeviceInfo(n); */
  /*   printf(" [%3d]: %s\n", n, di->name); */
  /* } */
  /* printf("\n"); */
  /* Pa_Terminate(); */

  printf("list of midi devices:\n");
#ifdef EDISON


  int midi_card = -1;
  int status;
  while(1)
  {
    status = snd_card_next(&midi_card);
    char *card_name;
    if(status < 0)
      printf("status: %d - %s -- card: %d\n", status, snd_strerror(status), midi_card);
    if(midi_card < 0)
      break;
    status = snd_card_get_longname(midi_card, &card_name);
    if(status < 0)
      printf("status: %d - %s -- card: %d\n", status, snd_strerror(status), midi_card);


    /* printf(" [%3d]: %s\n", midi_card, card_name); */

  }



#else
  Pm_Initialize();
  num_midi_devices = Pm_CountDevices();
  for(n = 0; n < num_midi_devices; n++)
  {
    const PmDeviceInfo *di = Pm_GetDeviceInfo(n);
    printf(" [%3d]: %s\n", n, di->name);
  }
  Pm_Terminate();
#endif

}
