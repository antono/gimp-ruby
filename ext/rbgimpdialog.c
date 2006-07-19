#include <ruby.h>
#include <gtk/gtk.h>

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include "rbgimp.h"

VALUE mRubyFu;

typedef struct
{
  GtkWidget *widget;
  VALUE (*result)(GtkWidget *widget);
} ValuePair;

static VALUE
get_entry_text (GtkWidget *widget)
{
  GtkEntry *entry = GTK_ENTRY(widget);
  return rb_str_new2(gtk_entry_get_text(entry));
}

static VALUE
get_spinner_int (GtkWidget *widget)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(widget);
  return INT2NUM(gtk_spin_button_get_value_as_int(spinner));
}

static VALUE
get_spinner_float (GtkWidget *widget)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(widget);
  return rb_float_new(gtk_spin_button_get_value(spinner));
}

static VALUE
get_color (GtkWidget *widget)
{
  GimpColorButton *cbutton = GIMP_COLOR_BUTTON(widget);
  
  GimpRGB color;
  gimp_color_button_get_color(cbutton, &color);
  volatile VALUE rbcolor = GimpRGB2rb(&color);
  
  return rbcolor;
}

static VALUE
get_bool (GtkWidget *widget)
{
  GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(widget);
  return INT2NUM(gtk_toggle_button_get_active(toggle) ? 1 : 0);
}

static VALUE
get_combo_box_int (GtkWidget *widget)
{
  gint value;
  gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(widget), &value);
  return INT2NUM(value);
}

static VALUE
get_font_name (GtkWidget *widget)
{
  gchar *str;
  g_object_get(G_OBJECT(widget), "font-name", &str, NULL);
  
  volatile VALUE rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}

static VALUE
get_filename(GtkWidget *widget)
{
  gchar *str = gimp_file_entry_get_filename(GIMP_FILE_ENTRY(widget));
  
  volatile VALUE  rbstr = rb_str_new2(str);
  g_free(str);
  
  return rbstr;
}

/*TODO remove this function*/
static VALUE
nothing(GtkWidget *widget)
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
make_color_button (VALUE deflt)
{
  GtkWidget *cbutton;
  GimpRGB *color = g_new(GimpRGB, 1);
  
  if (deflt == Qnil)
    gimp_rgba_set(color, 1.0, 1.0, 1.0, 1.0);
  else
    *color = rb2GimpRGB(deflt);
  
  cbutton = gimp_color_button_new("Ruby-Fu color selection", 60, 15, color,
                                  GIMP_COLOR_AREA_FLAT);
  gimp_color_button_set_update(GIMP_COLOR_BUTTON(cbutton), TRUE);
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

static ValuePair
handle_string_types (GimpPDBArgType type,
                     VALUE deflt,
                     VALUE param)
{
  ValuePair pair;
  char *defstr;
  if (deflt == Qnil)
    defstr = "";
  else
    defstr = StringValuePtr(deflt);
  
  VALUE subtype = rb_iv_get(param, "@subtype");
  if (subtype == Qnil)
  {
      pair.widget = gtk_entry_new();
      gtk_entry_set_text(GTK_ENTRY(pair.widget), defstr);
      pair.result = &get_entry_text;
  }
  
  ID subtypeid = SYM2ID(subtype);
  if (subtypeid == rb_intern("font"))
  {      
    pair.widget = gimp_font_select_button_new("Ruby-Fu font selection", defstr);
    pair.result = &get_font_name;
  }
  else if (subtypeid == rb_intern("file"))
  {
    pair.widget = gimp_file_entry_new("Ruby-Fu file selection",
                                      defstr, FALSE, TRUE);
    pair.result = &get_filename;
  }
  else if (subtypeid == rb_intern("palette"))
  {
  }
  else if (subtypeid == rb_intern("gradient"))
  {
  }
  else if (subtypeid == rb_intern("pattern"))
  {
  }
  else if (subtypeid == rb_intern("brush"))
  {
  }
    
  return pair;
}

static ValuePair
make_widget (VALUE param)
{
  ValuePair pair;

  GimpPDBArgType type = NUM2INT(rb_struct_aref(param, ID2SYM(id_type)));
  VALUE deflt = rb_iv_get(param, "@default");
  
  switch (type)
    {
    case GIMP_PDB_INT32:
      pair.widget = make_spinner(G_MININT32, G_MAXINT32, 1, deflt);
      pair.result = &get_spinner_int;
      break;
      
    case GIMP_PDB_INT16:
      pair.widget = make_spinner(G_MININT16, G_MAXINT16, 1, deflt);
      pair.result = &get_spinner_int;
      break;
      
    case GIMP_PDB_INT8:
      pair.widget = make_spinner(0, 255, 1, deflt);
      pair.result = &get_spinner_int;
      break;
      
    case GIMP_PDB_FLOAT:
      pair.widget = make_spinner(-G_MAXDOUBLE, G_MAXDOUBLE, 0.1, deflt);
      pair.result = &get_spinner_float;
      break;
      
    case GIMP_PDB_STRING:
      pair = handle_string_types(type, deflt, param);
      break;
      
    case GIMP_PDB_COLOR:
      pair.widget = make_color_button(deflt);
      pair.result = &get_color;
      break;
      
    case GIMP_PDB_IMAGE:
    case GIMP_PDB_DRAWABLE:
    case GIMP_PDB_CHANNEL:    
    case GIMP_PDB_LAYER:
      pair.widget = make_int_combo_box(type);
      if (deflt != Qnil)
        gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(pair.widget),
                                      NUM2INT(deflt));
      pair.result = &get_combo_box_int;
      break;
      
    default:
      pair.widget = gtk_label_new("Unimplemented");
      pair.result = &nothing;
      break;
    }

  return pair;
}

static GtkWidget *
make_table (VALUE       params,
            int        *num_pairs,
            ValuePair **pairs)
{
  params = rb_check_array_type(params);
  int num = RARRAY(params)->len;
  VALUE *ary = RARRAY(params)->ptr;

  ValuePair *pair_arr = g_new(ValuePair, num);

  GtkWidget *table = gtk_table_new(num, 2, FALSE);

  int i;
  for (i=0; i<num; i++)
    {
      VALUE param = ary[i];
      if (!rb_obj_is_kind_of(param, sGimpParamDef))
        rb_raise(rb_eArgError, "Parameters must be of type Gimp::ParamDef");

      VALUE name = rb_struct_aref(param, ID2SYM(id_name));
      GtkWidget *label = gtk_label_new(StringValuePtr(name));
      gtk_table_attach(GTK_TABLE(table), label,
                       0, 1, i, i+1,
                       GTK_FILL|GTK_EXPAND, GTK_FILL, 0, 0);

      ValuePair pair = make_widget(param);
      pair_arr[i] = pair;
      gtk_table_attach(GTK_TABLE(table), pair.widget,
                       1, 2, i, i+1,
                       GTK_FILL|GTK_EXPAND, GTK_FILL, 0, 0);
    }

  gtk_widget_show_all(table);

  *num_pairs = num;
  *pairs = pair_arr;
  return table;
}

static VALUE
collect_results (int        num_pairs,
                 ValuePair *pairs)
{
  volatile VALUE ary = rb_ary_new();
  int i;

  for (i=0; i<num_pairs; i++)
    {
      ValuePair pair = pairs[i];
      rb_ary_push(ary, pair.result(pair.widget));
    }

  return ary;
}

static VALUE
show_dialog (VALUE self,
             VALUE rbname,
             VALUE params)
{
  GtkWidget *dialog, *table;
  gchar *name = StringValuePtr(rbname);

  int num_pairs;
  ValuePair *pairs;

  gtk_init(NULL, NULL);
  gimp_ui_init ("ruby-fu", TRUE);

  dialog = gimp_dialog_new(name, "ruby-fu",
                           NULL, 0,
                           gimp_standard_help_func, "FIXME",
             
                           /*GIMP_STOCK_RESET, RESPONSE_RESET,*/
                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                           GTK_STOCK_OK,     GTK_RESPONSE_OK,
             
                           NULL);

  
  table = make_table(params, &num_pairs, &pairs);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), table, TRUE, TRUE, 0);
  
  
  if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
    return collect_results(num_pairs, pairs);

  return Qnil;
}

void Init_rubyfudialog(void)
{
  VALUE mRubyFu = rb_define_module("RubyFu");
  rb_define_module_function(mRubyFu, "dialog", show_dialog, 2);
}
