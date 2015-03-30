#include <stdio.h>
#include <portaudio.h>
#include <portmidi.h>


int main(int argc, char **argv)
{

  int n, num_audio_devices, num_midi_devices;

  printf("list of audio devices:\n");
  Pa_Initialize();
  num_audio_devices = Pa_GetDeviceCount();
  for(n = 0; n < num_audio_devices; n++)
  {
    const PaDeviceInfo *di = Pa_GetDeviceInfo(n);
    printf(" [%3d]: %s\n", n, di->name);
  }
  printf("\n");
  Pa_Terminate();

  printf("list of midi devices:\n");
  Pm_Initialize();
  num_midi_devices = Pm_CountDevices();
  for(n = 0; n < num_midi_devices; n++)
  {
    const PmDeviceInfo *di = Pm_GetDeviceInfo(n);
    printf(" [%3d]: %s\n", n, di->name);
  }
  Pm_Terminate();

}
