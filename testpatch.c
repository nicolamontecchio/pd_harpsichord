#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <z_libpd.h>


void pdprint(const char *s) {
  printf("%s\n", s);
  fflush(stdout);
}

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    libpd_set_printhook(pdprint);
    libpd_init();
    int init_err = libpd_init_audio(0, 2, 44100); // 0 in, 2 out, 44kHz
    if(init_err != 0)
    {
      printf("audio init failed with code %d\n", init_err);
      return 1;
    }
    libpd_start_message(1);
    libpd_add_float(1.0f);
    libpd_finish_message("pd", "dsp");

    void *patch = libpd_openfile(argv[1], ".");
    printf("patch file opened; handle: %d\n", (int) patch);
    while(1)
      sleep(10);
    libpd_closefile(patch);
    return 0;
  }
  else
  {
    printf("usage: %s [patchname].pd\n", argv[0]);
    return 1;
  }
}
