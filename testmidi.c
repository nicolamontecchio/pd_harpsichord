#include <stdio.h>
#include <stdlib.h>
#include <portmidi.h>

#define BUFLEN (100)

int main(int argc, char **argv)
{
  int midi_device_number = atoi(argv[1]);
  PortMidiStream *input_stream;
  PmEvent *msg_buffer;
  Pm_Initialize();
  msg_buffer = malloc(BUFLEN * sizeof(PmEvent));
  Pm_OpenInput(&input_stream, midi_device_number, NULL, BUFLEN, NULL, NULL);
  while(1)
  {
    if(Pm_Poll(input_stream))
    {
      int nread = Pm_Read(input_stream, msg_buffer, BUFLEN);
      for(int i = 0; i < nread; i++)
      {
	printf("timestamp: %d -- message: %d\n", msg_buffer[i].timestamp, msg_buffer[i].message);

      }
    }
  }
  Pm_Close(input_stream);
  free(msg_buffer);
  Pm_Terminate();
}
