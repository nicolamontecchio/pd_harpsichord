#include <stdlib.h>
#include "m_pd.h"

#ifdef EDISON
#include <mraa.h>
#endif

static t_class *mraagpio_class;

typedef struct _mraagpio
{
  t_object x_obj;
  t_clock *x_clock;
  int n_pins;
  int *pins;
  t_outlet *outlet_registers;
#ifdef EDISON
  mraa_gpio_context *gpios;
#endif
} t_mraagpio;

void mraagpio_tick(t_mraagpio *x)
{
#ifdef EDISON
  t_atom *output = malloc(sizeof(t_atom) * x->n_pins);
  int i;
  for (i = 0; i < x->n_pins; i++)
  {
    int status = mraa_gpio_read(x->gpios[i]);
    SETFLOAT(output + i, status);
  }
  outlet_list(x->outlet_registers, &s_list, x->n_pins, output);
  free(output);
#else
  /* // simulate randomly */
  /* int r = (rand() / 2) % 8; */
  /* for (int i = 0; i < x->n_pins; i++) */
  /* { */
  /*   SETFLOAT(output+i, r % 2); */
  /*   r /= 2; */
  /* } */
#endif
  clock_delay(x->x_clock, 1500);
}

void mraagpio_free(t_mraagpio *x)
{
  clock_free(x->x_clock);
  free(x->pins);
#ifdef EDISON
  mraa_deinit();
  free(x->gpios);
#endif
}

void *mraagpio_new(t_symbol *s, int argc, t_atom *argv)
{
  int i;
  t_mraagpio *x = (t_mraagpio *)pd_new(mraagpio_class);
  x->n_pins = argc;
  x->pins = malloc(sizeof(int) * argc);
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
      mraa_gpio_dir(x->gpios[i], MRAA_GPIO_IN);
#endif
    }
    else
    {
      post("error while creating mraagpio");
      return NULL;
    }
  }
  x->x_clock = clock_new(x, (t_method)mraagpio_tick);
  x->outlet_registers = outlet_new(&x->x_obj, &s_anything);
  clock_delay(x->x_clock, 100);
  return (x);
}

void mraagpio_setup(void)
{
  mraagpio_class = class_new(gensym("mraagpio"), (t_newmethod)mraagpio_new,
			     (t_method)mraagpio_free, sizeof(t_mraagpio), 0,
			     A_GIMME, 0);
}
