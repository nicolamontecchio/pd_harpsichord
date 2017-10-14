#include <stdio.h>
/* #include <portaudio.h> */
#include <alsa/asoundlib.h>

int main(int argc, char **argv) {

  int n, num_audio_devices, num_midi_devices;

   printf("ALSA library version: %s\n",
          SND_LIB_VERSION_STR);

  // TODO re-enable portaudio (or just rewrite directly into alsa?)

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

  int midi_card = -1;
  int status;
  while (1) // loop over sound cards
  {
    char *card_long_name;
    char card_name[128];
    snd_ctl_t *ctl;
    int midi_device = -1;
    status = snd_card_next(&midi_card);
    if (status < 0) {
      printf("status: %d - %s -- card: %d\n", status, snd_strerror(status),
             midi_card);
      return 1;
    }
    if (midi_card < 0)
      break;
    status = snd_card_get_longname(midi_card, &card_long_name);
    if (status < 0) {
      printf("status: %d - %s -- card: %d\n", status, snd_strerror(status),
             midi_card);
      return 1;
    }
    sprintf(card_name, "hw:%d", midi_card);
    status = snd_ctl_open(&ctl, card_name, 0);
    if (status < 0) {
      printf("snd_ctl_open failed with error %d: %s\n", status,
             snd_strerror(status));
      return 1;
    }
    while (1) // loop over devices
    {
      status = snd_ctl_rawmidi_next_device(ctl, &midi_device);
      if (status < 0) {
        printf("cannot determine device number: %s", snd_strerror(status));
        return 1;
      }
      if (midi_device < 0)
        break;
      printf(" [%3d]: %s\n", midi_card, card_long_name);
    }
    snd_ctl_close(ctl);
  }
}
