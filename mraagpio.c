#include "m_pd.h"



/* -------------------------- mraagpio ------------------------------ */
static t_class *mraagpio_class;

typedef struct _mraagpio
{
  t_object x_obj;
  t_clock *x_clock;
} t_mraagpio;

void mraagpio_tick(t_mraagpio *x)
{
  /* post("tick"); */
  /* clock_delay(x->x_clock, 1000); */

}

void mraagpio_free(t_mraagpio *x)
{
  clock_free(x->x_clock);
}

void *mraagpio_new(t_symbol *unitname, t_floatarg f, t_floatarg tempo)
{
  t_mraagpio *x = (t_mraagpio *)pd_new(mraagpio_class);
  /* mraagpio_ft1(x, f); */
  /* x->x_hit = 0; */
  x->x_clock = clock_new(x, (t_method)mraagpio_tick);
  outlet_new(&x->x_obj, gensym("bang"));
  /* inlet_new(&x->x_obj, &s_list) */
  /* if (tempo != 0) */
  /*   mraagpio_tempo(x, unitname, tempo); */

  return (x);
}

void mraagpio_setup(void)
{
  mraagpio_class = class_new(gensym("mraagpio"), (t_newmethod)mraagpio_new,
			     (t_method)mraagpio_free, sizeof(t_mraagpio), 0,
			     A_DEFFLOAT, A_DEFFLOAT, A_DEFSYM, 0);

  /* class_addbang(mraagpio_class, mraagpio_bang); */
  /* class_addmethod(mraagpio_class, (t_method)mraagpio_stop, gensym("stop"), 0); */
  /* class_addmethod(mraagpio_class, (t_method)mraagpio_ft1, gensym("ft1"), */
  /* 		  A_FLOAT, 0); */
  /* class_addmethod(mraagpio_class, (t_method)mraagpio_tempo, */
  /* 		  gensym("tempo"), A_FLOAT, A_SYMBOL, 0); */
  /* class_addfloat(mraagpio_class, (t_method)mraagpio_float); */
}
