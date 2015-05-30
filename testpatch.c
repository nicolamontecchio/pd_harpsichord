#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <z_libpd.h>

#ifdef EDISON
#include "pan.c"
#endif

void pdprint(const char *s) {
  printf("%s", s);
  fflush(stdout);
}

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    libpd_set_printhook(pdprint);
    libpd_init();
    pan_tilde_setup();

    void *patch = libpd_openfile(argv[1], ".");

    int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
    if(init_err != 0)
    {
      printf("audio init failed with code %d\n", init_err);
      return 1;
    }

    libpd_start_message(1);
    libpd_add_float(1.0f);
    libpd_finish_message("pd", "dsp");

    /* libpd_start_message(1); */
    /* libpd_add_float(1.0); */
    /* libpd_finish_message("fufi", "bang"); */



    printf("patch file opened; handle: %d\n", (int) patch);
    while(1)
    {
      printf("sleeping ...\n");
      sleep(1);
    }
    libpd_closefile(patch);
    return 0;
  }
  else
  {
    printf("usage: %s [patchname].pd\n", argv[0]);
    return 1;
  }
}
