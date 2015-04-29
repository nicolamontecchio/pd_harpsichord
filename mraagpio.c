#include "m_pd.h"

static t_class *mraagpio_class;


typedef struct _mraapgio
{
    t_object x_obj;
    t_clock *x_clock;
} t_mraagpio;

static void *mraagpio_new(t_symbol *selector, int argc, t_atom *argv)
{
  t_mraagpio *x = (t_mraagpio *)pd_new(mraagpio_class);

  return (void *)x;
}
