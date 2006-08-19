/* GIMP-Ruby -- Allows GIMP plugins to be written in Ruby.
 * Copyright (C) 2006  Scott Lembcke
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,Boston, MA
 * 02110-1301, USA.
 */

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
ID id_flags;

VALUE sGimpParamDef;
VALUE sGimpParam;
VALUE sGimpPlugInInfo;
VALUE sGimpParamRegion;
VALUE sGimpParasite;


void
Init_definitions (void)
{
  /* Define the id's */
  id_type = rb_intern("type");
  id_name = rb_intern("name");
  id_dscr = rb_intern("dscr");
  id_data = rb_intern("data");
  id_init_proc = rb_intern("init_proc");
  id_quit_proc = rb_intern("quit_proc");
  id_query_proc = rb_intern("query_proc");
  id_run_proc = rb_intern("run_proc");
  id_call = rb_intern("call");
  id_flags = rb_intern("flags");

  /* Define the structures */
  sGimpParamDef = rb_struct_define("GimpParamDef", "type", "name", "dscr", NULL);
  rb_define_const(mGimp, "ParamDef", sGimpParamDef);

  sGimpParam = rb_struct_define("GimpParam", "type", "data", NULL);
  rb_define_const(mGimp, "Param", sGimpParam);

  sGimpPlugInInfo = rb_struct_define("GimpPlugInInfo",
                                     "init_proc",
                                     "quit_proc",
                                     "query_proc",
                                     "run_proc", NULL);
  rb_define_const(mGimp, "PlugInInfo", sGimpPlugInInfo);
  
  sGimpParasite = rb_struct_define("GimpParasite",
                                   "name",
                                   "flags",
                                   "data", NULL);
  rb_define_const(mGimp, "Parasite", sGimpParasite);
}
