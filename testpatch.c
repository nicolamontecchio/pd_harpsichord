#include <stdio.h>
#include <strings.h>
#include <z_libpd.h>


void pdprint(const char *s) {
  printf("%s", s);
}

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    libpd_set_printhook(pdprint);
    libpd_init();
    void *patch = libpd_openfile(argv[1], ".");
    printf("patch file opened; handle: %d\n", (int) patch);
    libpd_closefile(patch);
    return 0;
  }
  else
  {
    printf("usage: %s [patchname].pd\n", argv[0]);
    return 1;
  }
}
