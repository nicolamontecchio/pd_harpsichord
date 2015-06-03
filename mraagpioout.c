#include <stdlib.h>
#include "m_pd.h"

#ifdef EDISON
#include <mraa.h>
#endif

static t_class *mraagpioout_class;

typedef struct _mraagpioout
{
  t_object x_obj;
  int n_pins;
  int *pins;
#ifdef EDISON
  mraa_gpio_context *gpios;
#endif
} t_mraagpioout;

void mraagpioout_free(t_mraagpioout *x)
{
  free(x->pins);
#ifdef EDISON
  free(x->gpios);
#endif
}

void mraagpioout_control_inlet(t_mraagpioout *x, t_symbol *selector, int argc, t_atom *argv)
{
  if(selector == &s_list)
  {
    int i;
    for(i = 0; i < (argc < x->n_pins ? argc : x->n_pins); i++)
    {
      int s = atom_getint(argv + i);
#ifdef EDISON
      mraa_gpio_write(x->gpios[i], s);
#endif
    }
  }
  else
  {
    post("mraagpioout: invalid message (must be list)");
  }
}

void *mraagpioout_new(t_symbol *s, int argc, t_atom *argv)
{
  int i;
  t_mraagpioout *x = (t_mraagpioout *)pd_new(mraagpioout_class);
  x->n_pins = argc;
  x->pins = (int *) malloc(sizeof(int) * argc);
#ifdef EDISON
  x->gpios = malloc(sizeof(mraa_gpio_context) * argc);
#endif
  for(i = 0; i < argc; i++)
  {
    if(argv[i].a_type == A_FLOAT)
    {
      x->pins[i] = atom_getint(argv + i);
#ifdef EDISON
      x->gpios[i] = mraa_gpio_init(x->pins[i]);
      mraa_gpio_dir(x->gpios[i], MRAA_GPIO_OUT);
#endif
    }
    else
    {
      post("error while creating mraagpioout");
      return NULL;
    }
  }
  printf("mraagpioout initialized\n"); fflush(stdout);
  return (x);
}

void mraagpioout_setup(void)
{
  mraagpioout_class = class_new(gensym("mraagpioout"), (t_newmethod)mraagpioout_new,
			     (t_method)mraagpioout_free, sizeof(t_mraagpioout), 0,
			     A_GIMME, 0);
  class_addanything(mraagpioout_class, (t_method)mraagpioout_control_inlet);
}
