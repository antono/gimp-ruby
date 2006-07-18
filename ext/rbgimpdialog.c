#include <ruby.h>
#include <gtk/gtk.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

typedef struct
{
  GtkWidget *widget;
  VALUE (*result)(GtkWidget *widget);
} ValuePair;

VALUE
get_text (GtkWidget *widget)
{
  GtkEntry *entry = GTK_ENTRY(widget);
  return rb_str_new2(gtk_entry_get_text(entry));
}

VALUE
get_int (GtkWidget *widget)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(widget);
  return INT2NUM(gtk_spin_button_get_value_as_int(spinner));
}

VALUE
get_float (GtkWidget *widget)
{
  GtkSpinButton *spinner = GTK_SPIN_BUTTON(widget);
  return rb_float_new(gtk_spin_button_get_value(spinner));
}

VALUE
get_bool (GtkWidget *widget)
{
  GtkToggleButton *toggle = GTK_TOGGLE_BUTTON(widget);
  return INT2NUM(gtk_toggle_button_get_active(toggle) ? 1 : 0);
}

ValuePair
make_widget (VALUE param)
{
  ValuePair pair;
  
  GimpPDBArgType type = NUM2INT(rb_struct_aref(param, ID2SYM(id_type)));
  
  switch(type)
    {
    case GIMP_PDB_STRING:
      pair.widget = gtk_entry_new();
      pair.result = &get_text;
      break;
    default:
    case GIMP_PDB_INT32:
      pair.widget = gtk_spin_button_new_with_range(-10, 10, 1);
      pair.result = &get_text;
      break;
    }
  
  return pair;
}

GtkWidget *
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

VALUE
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

VALUE
show_dialog (VALUE self,
             VALUE rbname,
             VALUE params)
{
  GtkWidget *dialog, *table;
  gchar *name = StringValuePtr(rbname);
  
  int num_pairs;
  ValuePair *pairs;

  gtk_init(NULL, NULL);
  
  dialog = gtk_dialog_new_with_buttons(name, NULL, 0,
                                       GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
                                       NULL);
  
  table = make_table(params, &num_pairs, &pairs);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), table, TRUE, TRUE, 0);
  
  
  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    return collect_results(num_pairs, pairs);
  
  return Qnil;
}

void Init_rubyfudialog(void)
{
  mRubyFu = rb_define_module("RubyFu");
  rb_define_module_function(mRubyFu, "dialog", show_dialog, 2);
}