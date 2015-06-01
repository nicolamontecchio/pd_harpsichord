#include "m_pd.h"
#include <string.h>

t_class *stoptrigger_class;

#define MAX_STOPS 256
const int NOTE_DELTA = 256; // first stop in range 0...NOTE_BASE-1, second in NOTE_BASE...2*NOTE_BASE-1, ...

typedef struct stoptrigger
{
  t_object x_ob;
  int nstops;                             // number of stops actually used
  int stop_active[MAX_STOPS];             // 1 if active, 0 otherwise
  t_outlet *outlet_notes;                 // main outlet
  t_outlet *outlet_stops;                 // outputs active stops
} t_stoptrigger;

const int STOPMSG_TYPE_ON = 0;
const int STOPMSG_TYPE_OFF = 1;
const int STOPMSG_TYPE_TOGGLE = 2;

void stop_printout(t_stoptrigger *x)
{
  int i;
  t_atom output[MAX_STOPS];
  for (i = 0; i < x->nstops; i++)
    SETFLOAT(output+i, x->stop_active[i]);
  outlet_list(x->outlet_stops,&s_list,x->nstops,output);
}

void stop_msg_process(t_stoptrigger *x, int type, int argc, t_atom *argv)
{
  int stop_id;
  if(argc != 1)
  {
    error("stoptrigger: missing argument");
    return;
  }
  stop_id = atom_getint(argv+0);
  if(stop_id < x->nstops && stop_id >= 0)
  {
    if(type == STOPMSG_TYPE_ON)
      x->stop_active[stop_id] = 1;
    else if(type == STOPMSG_TYPE_OFF)
      x->stop_active[stop_id] = 0;
    else if(type == STOPMSG_TYPE_TOGGLE)
      x->stop_active[stop_id] = x->stop_active[stop_id] > 0 ? 0 : 1;
  }
  else
  {
    error("stoptrigger: invalid stop specified");
    return;
  }
}

void stoptrigger_control_inlet(t_stoptrigger *x, t_symbol *selector, int argc, t_atom *argv)
{
  if(selector == &s_list)
  {
    int i;
    int note = atom_getint(argv+0);
    int velocity = atom_getint(argv+1);
    for (i = 0; i < x->nstops; i++)
      if(x->stop_active[i])
      {
	int outnote = note + NOTE_DELTA * i;
	t_atom output[2];
	SETFLOAT(output+0, outnote);
	SETFLOAT(output+1, velocity);
	outlet_list(x->outlet_notes,&s_list,2,output);
      }
  }
  else if(strcmp(selector->s_name, "stopon") == 0)
    stop_msg_process(x, STOPMSG_TYPE_ON, argc, argv);
  else if(strcmp(selector->s_name, "stopoff") == 0)
    stop_msg_process(x, STOPMSG_TYPE_OFF, argc, argv);
  else if(strcmp(selector->s_name, "stoptoggle") == 0)
    stop_msg_process(x, STOPMSG_TYPE_TOGGLE, argc, argv);
  else if(strcmp(selector->s_name, "stoplist") == 0)
    stop_printout(x);
}

// only one argument: the number of stops used
void *stoptrigger_new(t_symbol *selector, int argc, t_atom *argv)
{
  int i;
  t_stoptrigger *x = (t_stoptrigger *)pd_new(stoptrigger_class);
  if(argc != 1)
  {
    error("stoptrigger: must specify the number of stops");
    return 0;
  }
  x->nstops = atom_getint(argv + 0);
  if(x->nstops <= 0)
  {
    error("stoptrigger: number of stops must be a positive integer");
    return 0;
  }
  for (i = 0; i < x->nstops; i++)
    x->stop_active[i] = 0;
  x->outlet_notes = outlet_new(&x->x_ob, &s_list);
  x->outlet_stops = outlet_new(&x->x_ob, &s_list);
  return (void *)x;
}

void stoptrigger_setup(void)
{
  stoptrigger_class = class_new(gensym("stoptrigger"), (t_newmethod)stoptrigger_new,
				0, sizeof(t_stoptrigger), 0, A_GIMME, 0);
  class_addanything(stoptrigger_class, (t_method)stoptrigger_control_inlet);
}
