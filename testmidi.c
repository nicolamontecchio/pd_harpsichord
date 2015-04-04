#include <stdio.h>
#include <stdlib.h>
#include <portmidi.h>

#define BUFLEN (100)
#define MIDINOTEOFF 0x80
#define MIDINOTEON 0x90

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
	PmEvent msg = msg_buffer[i];
	int status = Pm_MessageStatus(msg.message);
	int data1  = Pm_MessageData1(msg.message);
	int data2  = Pm_MessageData2(msg.message);
	int data3  = ((msg.message >> 24) & 0xff);
	int msgtype = ((status & 0xf0) == 0xf0 ?
		       status : (status & 0xf0));

	/* printf("timestamp: %d -- message: %d\n", msg_buffer[i].timestamp, msg_buffer[i].message); */
	switch(msgtype)
	{
	case MIDINOTEON:
	  printf("note on:  %d %d\n", data1, data2);
	  break;
	case MIDINOTEOFF:
	  printf("note off: %d %d\n", data1, data2);
	  break;
	default:
	  printf("wtf\n");
	}
      }
    }
  }
  Pm_Close(input_stream);
  free(msg_buffer);
  Pm_Terminate();
}
