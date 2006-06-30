#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

ID id_type;
ID id_name;
ID id_dscr;
ID id_data;
ID id_init_proc;
ID id_quit_proc;
ID id_query_proc;
ID id_run_proc;
ID id_call;
ID id_x;
ID id_y;
ID id_width;
ID id_height;

void
Init_ids (void)
{
  id_type = rb_intern("type");
  id_name = rb_intern("name");
  id_dscr = rb_intern("dscr");
  id_data = rb_intern("data");
  id_init_proc = rb_intern("init_proc");
  id_quit_proc = rb_intern("quit_proc");
  id_query_proc = rb_intern("query_proc");
  id_run_proc = rb_intern("run_proc");
  id_call = rb_intern("call");
}
