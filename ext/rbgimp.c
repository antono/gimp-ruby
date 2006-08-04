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

VALUE mGimp;

static VALUE
rb_gimp_getpid(VALUE self)
{
  gint pid = gimp_getpid();
  return INT2NUM(pid);
}

static VALUE
rb_gimp_tile_width(VALUE self)
{
  gint width = gimp_tile_width();
  return INT2NUM(width);
}

static VALUE
rb_gimp_tile_height(VALUE self)
{
  gint height = gimp_tile_height();
  return INT2NUM(height);
}

static VALUE
rb_gimp_gamma(VALUE self)
{
  gdouble gamma = gimp_gamma();
  return rb_float_new(gamma);
}

static VALUE
rb_gimp_install_cmap(VALUE self)
{
  gboolean value = gimp_install_cmap();
  return value ? Qtrue : Qfalse;
}

static VALUE
rb_gimp_min_colors(VALUE self)
{
  gint num = gimp_min_colors();
  return INT2NUM(num);
}

static VALUE
rb_gimp_show_tool_tips(VALUE self)
{
  gboolean value = gimp_show_tool_tips();
  return value ? Qtrue : Qfalse;
}

static VALUE
rb_gimp_show_help_button(VALUE self)
{
  gboolean value = gimp_show_tool_tips();
  return value ? Qtrue : Qfalse;
}

static VALUE
rb_gimp_check_size(VALUE self)
{
  GimpCheckSize size = gimp_check_size();
  return INT2NUM(size);
}

static VALUE
rb_gimp_check_type(VALUE self)
{
  GimpCheckType type = gimp_check_type();
  return INT2NUM(type);
}

static VALUE
rb_gimp_default_display(VALUE self)
{
  gint32 id = gimp_default_display();
  return INT2NUM(id);
}

static VALUE
rb_gimp_wm_class(VALUE self)
{
  const gchar *class = gimp_wm_class();
  return rb_str_new2(class);
}

static VALUE
rb_gimp_display_name(VALUE self)
{
  const gchar *name = gimp_display_name();
  return rb_str_new2(name);
}

static VALUE
rb_gimp_monitor_number(VALUE self)
{
  gint num = gimp_monitor_number();
  return INT2NUM(num);
}

static VALUE
rb_gimp_get_progname(VALUE self)
{
  const gchar *name = gimp_get_progname();
  return rb_str_new2(name);
}

static VALUE
rb_gimp_gettext(VALUE self, VALUE rbstr)
{
  char *str = gettext(StringValuePtr(rbstr));
  return rb_str_new2(str);
}

void
Init_gimpext (void)
{
	mGimp = rb_define_module("Gimp");
  rb_define_module_function(mGimp, "getpid",           rb_gimp_getpid,           0);
  rb_define_module_function(mGimp, "tile_width",       rb_gimp_tile_width,       0);
  rb_define_module_function(mGimp, "tile_height",      rb_gimp_tile_height,      0);
  rb_define_module_function(mGimp, "gamma",            rb_gimp_gamma,            0);
  rb_define_module_function(mGimp, "install_cmap?",    rb_gimp_install_cmap,     0);
  rb_define_module_function(mGimp, "min_colors",       rb_gimp_min_colors,       0);
  rb_define_module_function(mGimp, "show_tool_tips?",  rb_gimp_show_tool_tips,   0);
  rb_define_module_function(mGimp, "show_help_button", rb_gimp_show_help_button, 0);
  rb_define_module_function(mGimp, "check_size",       rb_gimp_check_size,       0);
  rb_define_module_function(mGimp, "check_type",       rb_gimp_check_type,       0);
  rb_define_module_function(mGimp, "default_display",  rb_gimp_default_display,  0);
  rb_define_module_function(mGimp, "wm_class",         rb_gimp_wm_class,         0);
  rb_define_module_function(mGimp, "display_name",     rb_gimp_display_name,     0);
  rb_define_module_function(mGimp, "monitor_number",   rb_gimp_monitor_number,   0);
  rb_define_module_function(mGimp, "get_progname",     rb_gimp_get_progname,     0);
  
  rb_define_module_function(mGimp, "gettext", rb_gimp_gettext, 1);
  
  Init_structures();
  Init_ids();

  Init_gimpconstants();
  Init_gimpmain();
  Init_gimpproceduraldb();

  Init_gimpcolorrgb();
  Init_gimpcolorhsv();
  Init_gimpcolorcmyk();
  Init_gimpcolorspace();
  Init_rubyfudialog();
}
