#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

void register_enums(void)
{
  const gchar **enum_type_names;
  gint          n_enum_type_names;
  int i;
  
  g_type_init();
  gimp_enums_init();
  
  enum_type_names = gimp_enums_get_type_names (&n_enum_type_names);
  for (i = 0; i < n_enum_type_names; i++)
    {
      const gchar *enum_name  = enum_type_names[i];
      GType        enum_type  = g_type_from_name (enum_name);
      GEnumClass  *enum_class = g_type_class_ref (enum_type);
      GEnumValue  *value;
      
      VALUE mEnum = rb_define_module_under(mGimp, "EnumNames");
      volatile VALUE hash = rb_hash_new();
      rb_define_const(mEnum, enum_name + 4, hash);

      for (value = enum_class->values; value->value_name; value++)
        if (!strncmp ("GIMP_", value->value_name, 5))
          {
            gchar *name = value->value_name + 5;
            volatile VALUE rbname = rb_str_new2(name);
            VALUE num = INT2NUM(value->value);
            
            rb_hash_aset(hash, num, rbname);
            rb_define_const(mGimp, name, num);
          }

      g_type_class_unref (enum_class);
    }
}

void
Init_gimpconstants (void)
{
  register_enums();
  
  rb_define_const(mGimp, "MAJOR_VERSION", INT2NUM(GIMP_MAJOR_VERSION));
  rb_define_const(mGimp, "MINOR_VERSION", INT2NUM(GIMP_MINOR_VERSION));
  rb_define_const(mGimp, "MICRO_VERSION", INT2NUM(GIMP_MICRO_VERSION));
  rb_define_const(mGimp, "VERSION", rb_str_new2(GIMP_VERSION));
  rb_define_const(mGimp, "API_VERSION", rb_str_new2(GIMP_API_VERSION));
  
  rb_define_const(mGimp, "MIN_IMAGE_SIZE", INT2NUM(GIMP_MIN_IMAGE_SIZE));
  rb_define_const(mGimp, "MAX_IMAGE_SIZE", INT2NUM(GIMP_MAX_IMAGE_SIZE));
  rb_define_const(mGimp, "MIN_RESOLUTION", INT2NUM(GIMP_MIN_RESOLUTION));
  rb_define_const(mGimp, "MAX_RESOLUTION", INT2NUM(GIMP_MAX_RESOLUTION));
  
  rb_define_const(mGimp, "CHECK_SIZE", INT2NUM(GIMP_CHECK_SIZE));
  rb_define_const(mGimp, "CHECK_SIZE_SM", INT2NUM(GIMP_CHECK_SIZE_SM));
  rb_define_const(mGimp, "CHECK_DARK", INT2NUM(GIMP_CHECK_DARK));
  rb_define_const(mGimp, "CHECK_LIGHT", INT2NUM(GIMP_CHECK_LIGHT));
}
