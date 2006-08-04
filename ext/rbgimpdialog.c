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
#include <gtk/gtk.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <libintl.h>

#include "rbgimp.h"

VALUE mRubyFu;

typedef struct
{
  gpointer ptr;
  VALUE (*func)(gpointer ptr);
} Result;

static VALUE
get_entry_text (gpointer ptr)
{
  GtkEntry *entry = GTK_ENTRY(ptr);
  return rb_str_new2(gtk_entry_get_text(entry));
}

static VALUE
get_text_buffer_string (gpointer ptr)
{
  GtkTextBuffer *text_buffer = ptr;
  GtkTextIter start, end;
  
  gtk_text_buffer_get_start_iter(text_buffer, &start);
  gtk_text_buffer_get_end_iter(text_buffer, &end);
  
  gchar *str = gtk_text_buffer_get_text(text_buffer, &start, &end, TRUE);
  VALUE rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}

static VALUE
get_spinner_int (gpointer ptr)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(ptr);
  return INT2NUM(gtk_spin_button_get_value_as_int(spinner));
}

static VALUE
get_toggle_int (gpointer ptr)
{
  GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(ptr);
  return INT2NUM(gtk_toggle_button_get_active(toggle) ? 1 : 0);
}

static VALUE
get_spinner_float (gpointer ptr)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(ptr);
  return rb_float_new(gtk_spin_button_get_value(spinner));
}

static VALUE
get_slider_float (gpointer ptr)
{
  GtkRange *range = GTK_RANGE(ptr);
  return rb_float_new(gtk_range_get_value(range));
}

static VALUE
get_color (gpointer ptr)
{
  GimpColorButton *cbutton = GIMP_COLOR_BUTTON(ptr);
  
  GimpRGB color;
  gimp_color_button_get_color(cbutton, &color);
  volatile VALUE rbcolor = GimpRGB2rb(&color);
  
  return rbcolor;
}

static VALUE
get_combo_box_int (gpointer ptr)
{
  gint value;
  gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(ptr), &value);
  return INT2NUM(value);
}

static VALUE
get_combo_box_string (gpointer ptr)
{
  gchar *str = gtk_combo_box_get_active_text(GTK_COMBO_BOX(ptr));
  VALUE rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}
  

static VALUE
get_font_name (gpointer ptr)
{
  gchar *str;
  g_object_get(G_OBJECT(ptr), "font-name", &str, NULL);
  
  volatile VALUE rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}

static VALUE
get_filename(gpointer ptr)
{
  gchar *str = gimp_file_entry_get_filename(GIMP_FILE_ENTRY(ptr));
  
  volatile VALUE  rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}

static VALUE
get_string_from_pointer(gpointer ptr)
{
  gchar **str = ptr;
  volatile VALUE rbstr;
  
  if (*str)
   rbstr = rb_str_new2(*str);
  else
    rbstr = rb_str_new(NULL, 0);
  
  g_free(*str);
  g_free(ptr);
  
  return rbstr;
}

static void
palette_select (const gchar *name,
                gboolean closing,
                gpointer data)
{
  gchar **str = data;
  g_free(*str);
  *str = g_strdup(name);
}

static void
gradient_select (const gchar *name,
                 gint width,
                 const gdouble *grad_data,
                 gboolean dialog_closing,
                 gpointer data)
{
  gchar **str = data;
  g_free(*str);
  *str = g_strdup(name);
}

static void
pattern_select (const gchar *name,
                gint width,
                gint height,
                gint bpp,
                const guchar *mask_data,
                gboolean dialog_closing,
                gpointer data)
{
  gchar **str = data;
  g_free(*str);
  *str = g_strdup(name);
}

static void
brush_select (const gchar *name,
              gdouble opacity,
              gint spacing,
              GimpLayerModeEffects mode,
              gint width,
              gint height,
              const guchar *mask_data,
              gboolean dialog_closing,
              gpointer data)
{
  gchar **str = data;
  g_free(*str);
  *str = g_strdup(name);
}

/*TODO remove this function*/
static VALUE
nothing (gpointer ptr)
{
  return Qnil;
}

static GtkWidget *
make_spinner (gdouble min,
              gdouble max,
              gdouble step,
              VALUE deflt)
{
  GtkWidget *spinner;
  spinner = gtk_spin_button_new_with_range(min, max, step);
  
  if (deflt != Qnil)
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinner), NUM2DBL(deflt));
    
  return spinner;
}

static GtkWidget *
make_color_button (VALUE deflt, Result *result)
{
  GtkWidget *cbutton;
  GimpRGB *color = g_new(GimpRGB, 1);
  
  if (deflt == Qnil)
    gimp_rgba_set(color, 1.0, 1.0, 1.0, 1.0);
  else
    *color = rb2GimpRGB(deflt);
  
  cbutton = gimp_color_button_new("Ruby-Fu", 60, 15, color,
                                  GIMP_COLOR_AREA_FLAT);
  gimp_color_button_set_update(GIMP_COLOR_BUTTON(cbutton), TRUE);
  
  g_free(color);
  
  result->ptr = cbutton;
  result->func = &get_color;
  return cbutton;
}

static GtkWidget *
make_int_combo_box (GimpPDBArgType type)
{
  switch (type)
    {
    case GIMP_PDB_IMAGE:
      return gimp_image_combo_box_new(NULL, NULL);
      
    case GIMP_PDB_DRAWABLE:
      return gimp_drawable_combo_box_new(NULL, NULL);
      
    case GIMP_PDB_CHANNEL:
      return gimp_channel_combo_box_new(NULL, NULL);
    
    case GIMP_PDB_LAYER:
      return gimp_layer_combo_box_new(NULL, NULL);
    
    default:
      return NULL;
    }
}

static GtkWidget *
make_list_combo_box (VALUE list)
{
  GtkWidget *widget = gtk_combo_box_new_text();
  GtkComboBox *cbox = GTK_COMBO_BOX(widget);
  
  list = rb_check_array_type(list);
  int num = RARRAY(list)->len;
  VALUE *ary = RARRAY(list)->ptr;
  
  int i;
  for (i=0; i<num; i++)
    gtk_combo_box_append_text(cbox, StringValuePtr(ary[i]));
  
  gtk_combo_box_set_active(cbox, 0);
  
  return widget;
}

static GtkWidget *
make_text_box (gchar *deflt, gpointer *buffer)
{
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);
  
  GtkTextBuffer *text_buffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(text_buffer, deflt, -1);
  *buffer = text_buffer;
  
  GtkWidget *text_view = gtk_text_view_new_with_buffer(text_buffer);
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);
  
  return scrolled_window;
}

static gchar **
wrap_string(gchar *str)
{
  gchar **ptr = g_new(gchar*, 1);
  *ptr = g_strdup(str);

  return ptr;
}

static GtkWidget *
handle_string_types (VALUE param,
                     VALUE deflt,
                     Result *result)
{
  GtkWidget *widget;
  gpointer data;
  
  gchar *defstr;
  if (deflt == Qnil)
    defstr = "";
  else
    defstr = StringValuePtr(deflt);
  
  VALUE subtype = rb_iv_get(param, "@subtype");
  if (subtype == Qnil)
    {
      widget = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(widget), defstr);
      
      result->ptr = widget;
      result->func = &get_entry_text;
      return widget;
    }
  
  ID subtypeid = SYM2ID(subtype);
  if (subtypeid == rb_intern("text"))
    {
      widget = make_text_box(defstr, &result->ptr);
      result->func = &get_text_buffer_string;
    }
  else if (subtypeid == rb_intern("font"))
    {
      widget = gimp_font_select_button_new("Ruby-Fu", defstr);
      
      result->ptr = widget;
      result->func = &get_font_name;
    }
  else if (subtypeid == rb_intern("file"))
    {
      widget = gimp_file_entry_new("Ruby-Fu", defstr, FALSE, TRUE);
      
      result->ptr = widget;
      result->func = &get_filename;
    }
  else if (subtypeid == rb_intern("dir"))
    {
      widget = gimp_file_entry_new("Ruby-Fu", defstr, TRUE, TRUE);
      
      result->ptr = widget;
      result->func = &get_filename;
    }
  else if (subtypeid == rb_intern("palette"))
    {
      widget = gimp_palette_select_widget_new("Ruby-Fu", defstr,
                                              &palette_select, data);
      
      result->ptr = wrap_string(defstr);
      result->func = &get_string_from_pointer;
    }
  else if (subtypeid == rb_intern("gradient"))
    {
      widget = gimp_gradient_select_widget_new("Ruby-Fu", defstr,
                                               &gradient_select, data);
      
     result->ptr = wrap_string(defstr);
     result->func = &get_string_from_pointer;
    }
  else if (subtypeid == rb_intern("pattern"))
    {
      widget = gimp_pattern_select_widget_new("Ruby-Fu", defstr,
                                              &pattern_select, data);
  
      result->ptr = wrap_string(defstr);
      result->func = &get_string_from_pointer;
    }
  else if (subtypeid == rb_intern("brush"))
    {
      widget = gimp_brush_select_widget_new("Ruby-Fu", defstr,
                                            100, -1, GIMP_NORMAL_MODE,
                                            &brush_select, data);
      
      result->ptr = wrap_string(defstr);
      result->func = &get_string_from_pointer;
    }
  else if (subtypeid == rb_intern("list"))
    {
      VALUE list = rb_iv_get(param, "@list");
      widget = make_list_combo_box(list);
      
      result->ptr = widget;
      result->func = &get_combo_box_string;
    }
  else
    {
      widget = NULL;
      rb_raise(rb_eTypeError, "Bad RubyFu::ParamDef string subtype.");
    }
  
  return widget;
}

static GtkWidget *
handle_int_types (VALUE param,
                  VALUE deflt,
                  Result *result)
{
  GtkWidget *widget;
  
  gint defint;
  if (deflt == Qnil)
    defint = 0;
  else
    defint = NUM2INT(deflt);
  
  VALUE subtype = rb_iv_get(param, "@subtype");
  if (subtype == Qnil)
    {
      widget = make_spinner(G_MININT32, G_MAXINT32, 1, deflt);
      
      result->ptr = widget;
      result->func = &get_spinner_int;
      return widget;
    }
  
  ID subtypeid = SYM2ID(subtype);
  if (subtypeid == rb_intern("toggle"))
    {
      widget = gtk_check_button_new();
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), defint);
      
      result->ptr = widget;
      result->func = &get_toggle_int;
    }
  else if (subtypeid == rb_intern("enum"))
    {
      VALUE enum_name = rb_iv_get(param, "@enum");
      GType enum_type = g_type_from_name(StringValuePtr(enum_name));
      widget = gimp_enum_combo_box_new(enum_type);
      gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(widget),
                                    defint);
      
      result->ptr = widget;
      result->func = &get_combo_box_int;
    }
  else
    {
      widget = NULL;
      rb_raise(rb_eTypeError, "Bad RubyFu::ParamDef int subtype.");
    }

  return widget;
}

static void
get_min_max_step(VALUE param,
                 VALUE *min,
                 VALUE *max,
                 VALUE *step)
{
  VALUE range = rb_iv_get(param, "@range");
  
  *step = rb_iv_get(param, "@step");
  *min = rb_funcall(range, rb_intern("begin"), 0, NULL);
  *max = rb_funcall(range, rb_intern("end"), 0, NULL);
}

static GtkWidget *
handle_float_types (VALUE param,
                    VALUE deflt,
                    Result *result)
{
  GtkWidget *widget;
  
  VALUE subtype = rb_iv_get(param, "@subtype");
  if (subtype == Qnil)
    {
      widget = make_spinner(-G_MAXDOUBLE, G_MAXDOUBLE, 0.001, deflt);
      
      result->ptr = widget;
      result->func = &get_spinner_float;
      return widget;
    }
  
  ID subtypeid = SYM2ID(subtype);
  if (subtypeid == rb_intern("spinner"))
    {
      VALUE min, max, step;
      get_min_max_step(param, &min, &max, &step);
            
      widget = make_spinner(NUM2DBL(min), NUM2DBL(max), NUM2DBL(step), deflt);
      
      result->ptr = widget;
      result->func = &get_spinner_float;
    }
  else if (subtypeid == rb_intern("slider"))
    {
      VALUE min, max, step;
      get_min_max_step(param, &min, &max, &step);
      
      widget = gtk_hscale_new_with_range(NUM2DBL(min),
                                         NUM2DBL(max),
                                         NUM2DBL(step));
      
      gtk_range_set_value(GTK_RANGE(widget), NUM2DBL(deflt));
      
      result->ptr = widget;
      result->func = &get_slider_float;
    }
  else
    {
      widget = NULL;
      rb_raise(rb_eTypeError, "Bad RubyFu::ParamDef float subtype.");
    }

  return widget;
}

static GtkWidget *
make_widget (VALUE param,
             Result *result)
{
  GtkWidget *widget;

  GimpPDBArgType type = NUM2INT(rb_struct_aref(param, ID2SYM(id_type)));
  VALUE deflt = rb_iv_get(param, "@default");
  
  switch (type)
    {
    case GIMP_PDB_INT32:
      widget = handle_int_types(param, deflt, result);
      break;
      
    case GIMP_PDB_INT16:
      widget = make_spinner(G_MININT16, G_MAXINT16, 1, deflt);
      
      result->ptr = widget;
      result->func = &get_spinner_int;
      break;
      
    case GIMP_PDB_INT8:
      widget = make_spinner(0, 255, 1, deflt);
      
      result->ptr = widget;
      result->func = &get_spinner_int;
      break;
      
    case GIMP_PDB_FLOAT:
      widget = handle_float_types(param, deflt, result);
      break;
      
    case GIMP_PDB_STRING:
      widget = handle_string_types(param, deflt, result);
      break;
      
    case GIMP_PDB_COLOR:
      widget = make_color_button(deflt, result);
      break;
      
    case GIMP_PDB_IMAGE:
    case GIMP_PDB_DRAWABLE:
    case GIMP_PDB_CHANNEL:    
    case GIMP_PDB_LAYER:
      widget = make_int_combo_box(type);
      
      if (deflt != Qnil)
        gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(widget),
                                      NUM2INT(deflt));
      
      result->ptr = widget;
      result->func = &get_combo_box_int;
      break;
      
    default:
      widget = gtk_label_new("Unimplemented");
      
      result->ptr = widget;
      result->func = &nothing;
      break;
    }

  return widget;
}

static GtkWidget *
make_table (VALUE    params,
            int     *num_results,
            Result **results)
{
  params = rb_check_array_type(params);
  int num = RARRAY(params)->len;
  VALUE *ary = RARRAY(params)->ptr;

  Result *results_arr = g_new(Result, num);

  GtkWidget *table = gtk_table_new(num+1, 2, FALSE);
  gtk_container_set_border_width(GTK_CONTAINER(table), 12);
  gtk_table_set_col_spacings(GTK_TABLE(table), 6);
  gtk_table_set_row_spacings(GTK_TABLE(table), 6);

  int i;
  for (i=0; i<num; i++)
    {
      VALUE param = ary[i];
      if (!rb_obj_is_kind_of(param, sGimpParamDef))
        rb_raise(rb_eArgError, "Parameters must be of type Gimp::ParamDef");

      VALUE rbdscr = rb_struct_aref(param, ID2SYM(id_dscr));
      gchar *dscr = g_strdup_printf("%s:", gettext(StringValuePtr(rbdscr)));
      
      GtkWidget *label = gtk_label_new(dscr);
      g_free(dscr);
      
      gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);
      gtk_table_attach(GTK_TABLE(table), label, 0, 1, i, i+1,
                       GTK_FILL, GTK_FILL, 0, 0);

      GtkWidget *widget = make_widget(param, &results_arr[i]);
      gtk_table_attach(GTK_TABLE(table), widget, 1, 2, i, i+1,
                       GTK_FILL|GTK_EXPAND, GTK_FILL, 0, 0);
    }
  
  gtk_table_attach(GTK_TABLE(table), gimp_progress_bar_new(),
                   0, 2, num, num+1,
                   GTK_FILL|GTK_EXPAND, GTK_FILL, 0, 0);
  gtk_widget_show_all(table);

  *num_results = num;
  *results = results_arr;
  return table;
}

static VALUE
collect_results (int     num_results,
                 Result *results)
{
  volatile VALUE ary = rb_ary_new();
  int i;

  for (i=0; i<num_results; i++)
    {
      Result result = results[i];
      rb_ary_push(ary, result.func(result.ptr));
    }

  return ary;
}

static VALUE
show_dialog (VALUE self,
             VALUE rbtitle,
             VALUE rbprocname,
             VALUE params)
{
  GtkWidget *dialog, *table;
  gchar *procname = StringValuePtr(rbprocname);
  gchar *title = g_strdup_printf("Ruby Fu: %s",
                                 gettext(StringValuePtr(rbtitle)));

  int num_results;
  Result *results;

  gtk_init(NULL, NULL);
  gimp_ui_init ("ruby-fu", TRUE);

  dialog = gimp_dialog_new(title, "rubyfu", NULL, 0,
                           gimp_standard_help_func, procname,
             
                           /*GIMP_STOCK_RESET, RESPONSE_RESET,*/
                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                           GTK_STOCK_OK,     GTK_RESPONSE_OK,
             
                           NULL);
  g_free(title);

  gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), 12);
  
  table = make_table(params, &num_results, &results);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), table, TRUE, TRUE, 0);
  
  gint response = gimp_dialog_run(GIMP_DIALOG(dialog));
  volatile VALUE rbresults = collect_results(num_results, results);
  if (response == GTK_RESPONSE_OK)
    return rbresults;

  return Qnil;
}

void Init_rubyfudialog(void)
{
  VALUE mRubyFu = rb_define_module("RubyFu");
  rb_define_module_function(mRubyFu, "dialog", show_dialog, 3);
}
