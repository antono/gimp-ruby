/* GIMP-Ruby -- Allows GIMP plugins to be written in Ruby.
 * Copyright (C) 2006  Scott Lembcke
 * Copyright (C) 2011  Antono Vasiljev
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

#include <stdlib.h>
#include <stdio.h>

#include <ruby.h>
#include <libgimp/gimp.h>

/* It seems that ruby has it's own config.h file that it includes.
 * We have to undef some things so we don't get name collisions */
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#include <libintl.h>
/* including "config.h" would include Ruby's config.h */
#include "../config.h"

#include "rbgimp.h"

static VALUE gc_array;

typedef struct {
  GimpParam *array;
  gint count;
} ParamsWrapper;

static void
params_wrapper_free (ParamsWrapper *ptr)
{
  gimp_destroy_params(ptr->array, ptr->count);
}

static void
gc_register_params (GimpParam *array, gint count)
{
  ParamsWrapper *ptr = ALLOC(ParamsWrapper);
  ptr->array = array;
  ptr->count = count;
  
  volatile VALUE obj;
  obj = Data_Wrap_Struct(rb_cObject, NULL, params_wrapper_free, ptr);
  
  rb_ary_push(gc_array, obj);
}

void
handle_exception (char *message)
{
  VALUE rbmessage;
  rbmessage = rb_eval_string("\"\nA #{$!.class} has occured: #{$!.message}\n#{$@.join(\"\n\")}\"");
  char *str = StringValuePtr(rbmessage);
  
  gchar *result = g_strconcat(message, str, NULL);
  gimp_message(result);
  g_free(result);
  
  gimp_quit();
}

VALUE plug_in_callbacks[4];

static VALUE
init_protect (VALUE unused)
{
  VALUE cback = plug_in_callbacks[0];
	
  if (!rb_respond_to(cback, id_call))
    rb_raise(rb_eTypeError, "The init callback does not respond to #call");

  rb_funcall(cback, id_call, 0);
  return Qnil;
}

static void
init_callback (void)
{
  int err;
  rb_protect(init_protect, Qnil, &err);
  
  if (err)
    handle_exception("An exception has occurred while running the init callback.");
}

static VALUE
quit_protect (VALUE unused)
{
  VALUE cback = plug_in_callbacks[1];
	
  if (!rb_respond_to(cback, id_call))
    rb_raise(rb_eTypeError, "The quit callback does not respond to #call");

  rb_funcall(cback, id_call, 0);
  return Qnil;
}

static void
quit_callback (void)
{
  int err;
  rb_protect(quit_protect, Qnil, &err);
  
  if (err)
    handle_exception("An exception has occurred while running the quit callback.");
}

static VALUE
query_protect (VALUE unused)
{
  VALUE cback = plug_in_callbacks[2];
  
  if (!rb_respond_to(cback, id_call))
    rb_raise(rb_eTypeError, "The query callback does not respond to #call");

  rb_funcall(cback, id_call, 0);
  return Qnil;
}

static void
query_callback (void)
{
  int err;
  rb_protect(query_protect, Qnil, &err);
  
  if (err)
    handle_exception("An exception has occurred while running the query callback");
}

static VALUE
run_protect (VALUE args)
{
  VALUE cback = plug_in_callbacks[3];
  if (rb_respond_to(cback, id_call))
    {
      volatile VALUE rbreturn_vals;
      rbreturn_vals = rb_apply(cback, id_call, args);
      return rb_check_array_type(rbreturn_vals);
    }
  else
    {
      rb_raise(rb_eTypeError, "The run callback does not respond to #call");
      return Qnil;
    }
}

static void
run_callback (const gchar      *name,
              gint             n_params,
              const GimpParam  *param,
              gint             *n_return_vals,
              GimpParam       **return_vals)
{
  volatile VALUE args = GimpParams2rb(param, n_params);
  rb_ary_unshift(args, rb_str_new2(name));
  
  int err;
  volatile VALUE rbreturn_vals;
  rbreturn_vals = rb_protect(run_protect, args, &err);
  
  if (err)
    handle_exception("An exception has occured while running the run callback");
  
  /*FIXME rb2GimpParams can raise exceptions*/
  *return_vals = rb2GimpParams(rbreturn_vals, n_return_vals);
  gc_register_params(*return_vals, *n_return_vals);
}

static VALUE
rb_gimp_main (VALUE self,
             VALUE plug_in_info)
{
  VALUE init_proc  = rb_struct_aref (plug_in_info, ID2SYM(id_init_proc));
  VALUE quit_proc  = rb_struct_aref (plug_in_info, ID2SYM(id_quit_proc));
  VALUE query_proc = rb_struct_aref (plug_in_info, ID2SYM(id_query_proc));
  VALUE run_proc   = rb_struct_aref (plug_in_info, ID2SYM(id_run_proc));
  
  plug_in_callbacks[0] = init_proc;
  plug_in_callbacks[1] = quit_proc;
  plug_in_callbacks[2] = query_proc;
  plug_in_callbacks[3] = run_proc;
  
  /* build argv */
  VALUE *arr = RARRAY_PTR(RARRAY(rb_argv));
  gint argc = RARRAY_LEN(RARRAY(rb_argv));
  gchar *argv[argc + 1];
  
  VALUE name = rb_gv_get("$0");
  argv[0] = StringValuePtr(name);
  
  int i;
  for (i=0; i<argc; i++)
          argv[i + 1] = StringValuePtr(arr[i]);
  
  /* call gimp_main */
  GimpPlugInInfo plug_in_info_struct = {
    (init_proc != Qnil) ? init_callback : NULL,
    (quit_proc != Qnil) ? quit_callback : NULL,
    query_callback,
    run_callback
  };
  
  gint rstatus = gimp_main(&plug_in_info_struct, argc + 1, argv);
  return INT2NUM(rstatus);
}

static VALUE
rb_gimp_quit (VALUE self)
{
  gimp_quit();
  return Qnil;
}

static VALUE
rb_gimp_install_procedure (VALUE self,
                           VALUE name,
                           VALUE blurb,
                           VALUE help,
                           VALUE author,
                           VALUE copyright,
                           VALUE date,
                           VALUE menu_label,
                           VALUE image_types,
                           VALUE type,
                           VALUE params,
                           VALUE return_vals)
{
  gint n_params;
  GimpParamDef *paramdefs;
  paramdefs = rb2GimpParamDefs(params, &n_params);
  
  gint n_return_vals;
  GimpParamDef *returndefs;
  returndefs = rb2GimpParamDefs(return_vals, &n_return_vals);
		
  gimp_install_procedure((gchar *)StringValuePtr(name),
                         (gchar *)StringValuePtr(blurb),
                         (gchar *)StringValuePtr(help),
                         (gchar *)StringValuePtr(author),
                         (gchar *)StringValuePtr(copyright),
                         (gchar *)StringValuePtr(date),
                         (gchar *)StringValuePtr(menu_label),
                         (gchar *)StringValuePtr(image_types),
                         (GimpPDBProcType)NUM2INT(type),
                         n_params,
                         n_return_vals,
                         paramdefs,
                         returndefs);
  
  g_free(paramdefs);
  g_free(returndefs);
  
  return Qnil;
}

static VALUE
rb_gimp_run_procedure (VALUE self,
                       VALUE name,
                       VALUE rbparams)
{
  gint n_params;
  GimpParam *params = rb2GimpParams(rbparams, &n_params);
  
  gint n_return_vals;
  GimpParam *return_vals;
  return_vals = gimp_run_procedure2((gchar *)StringValuePtr(name),
                                    &n_return_vals,
                                    n_params,
                                    params);
  
  volatile VALUE rbreturn_vals;
  rbreturn_vals = GimpParams2rb(return_vals, n_return_vals);
  
  gimp_destroy_params(params, n_params);
  gimp_destroy_params(return_vals, n_return_vals);
  
  return rbreturn_vals;
}

void
Init_gimpmain (void)
{
  gc_array = rb_ary_new();
  rb_gc_register_address(&gc_array);
    
  bindtextdomain(GETTEXT_PACKAGE, gimp_locale_directory());
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  textdomain(GETTEXT_PACKAGE);
  
  rb_define_module_function(mGimp, "main", rb_gimp_main, 1);
  rb_define_module_function(mGimp, "quit", rb_gimp_quit, 0);
  rb_define_module_function(mGimp, "install_procedure", rb_gimp_install_procedure, 11);
  rb_define_module_function(mGimp, "run_procedure", rb_gimp_run_procedure, 2);
}
