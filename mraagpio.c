#include <stdlib.h>
#include "m_pd.h"


static t_class *mraagpio_class;

typedef struct _mraagpio
{
  t_object x_obj;
  t_clock *x_clock;
  int *pins;
  int n_pins;
} t_mraagpio;

void mraagpio_tick(t_mraagpio *x)
{
  post("tick");

  clock_delay(x->x_clock, 100);
}

void mraagpio_free(t_mraagpio *x)
{
  clock_free(x->x_clock);
  free(x->pins);
}

void *mraagpio_new(t_symbol *s, int argc, t_atom *argv)
{
  t_mraagpio *x = (t_mraagpio *)pd_new(mraagpio_class);
  x->n_pins = argc;
  x->pins = malloc(sizeof(int) * argc);
  for(int i = 0; i < argc; i++)
  {
    if(argv[i].a_type == A_FLOAT)
      x->pins[i] = atom_getint(argv + i);
    else
    {
      post("error while creating mraagpio");
      return NULL;
    }
  }
  x->x_clock = clock_new(x, (t_method)mraagpio_tick);
  outlet_new(&x->x_obj, gensym("bang"));
  clock_delay(x->x_clock, 100);
  return (x);
}

void mraagpio_setup(void)
{
  mraagpio_class = class_new(gensym("mraagpio"), (t_newmethod)mraagpio_new,
			     (t_method)mraagpio_free, sizeof(t_mraagpio), 0,
			     A_GIMME, 0);
}
