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

  int maxcount = 1000;   // Exit after this many bytes have been received.
  int count = 0;         // Current count of bytes received.
  char buffer[1];        // Storage for input buffer received
  while (count < maxcount) {
    status = snd_rawmidi_read(midiin, buffer, 1);
    if (status == -11)
    {
      // to nothing
    }
    else if (status < 0) {
      printf("Problem reading MIDI input [%d]: %s\n", status, snd_strerror(status));
    }
    else
    {
      count++;
      if ((unsigned char)buffer[0] >= 0x80) {   // command byte: print in hex
	printf("0x%x ", (unsigned char)buffer[0]);
      } else {
	printf("%d ", (unsigned char)buffer[0]);
      }
      fflush(stdout);
      if (count % 20 == 0) {  // print a newline to avoid line-wrapping
	printf("\n");
      }
    }
  }
  snd_rawmidi_close(midiin);

}
