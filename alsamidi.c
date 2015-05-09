#include <alsa/asoundlib.h>

const int CARD = 2;

int main(int argc, char *argv[])
{
  char *longname = NULL;
  snd_card_get_longname(CARD, &longname);
  printf("card long name: %s\n", longname);

  char portname[32];
  sprintf(portname, "hw:%d", CARD);
  snd_ctl_t *ctl;
  snd_ctl_open(&ctl, portname, 0);

  snd_rawmidi_t* midiin = NULL;
  int status = snd_rawmidi_open(&midiin, NULL, portname, SND_RAWMIDI_NONBLOCK);
  printf("status %d\n", status);

  int note_cycle = 0; // in [0,3)
  char note_message[3];

  int maxcount = 1000;   // Exit after this many bytes have been received.
  int count = 0;         // Current count of bytes received.
  int i;

  char buffer[1];        // Storage for input buffer received

  while (count < maxcount) {
    status = snd_rawmidi_read(midiin, buffer, 1);
    if (status == -11) { } // do nothing
    else if (status < 0) {
      printf("Problem reading MIDI input [%d]: %s\n", status, snd_strerror(status));
    }
    else
    {
      count++;
      if ((unsigned char)buffer[0]>= 0x80) // command byte
      {
      	note_cycle = 0;
	note_message[note_cycle++] = buffer[0];
      }
      else
      {
	note_message[note_cycle++] = buffer[0];
      	if(note_cycle == 3)
      	{
	  printf("0x%x   %3d %3d\n", note_message[0], note_message[1], note_message[2]);
      	  fflush(stdout);
      	  note_cycle = 0;
      	}
      }
    }
  }
  snd_rawmidi_close(midiin);

}
