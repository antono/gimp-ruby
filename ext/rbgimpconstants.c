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

void register_enums(void)
{
  const gchar **enum_type_names;
  gint          n_enum_type_names;
  int i;
  
  g_type_init();
  gimp_enums_init();
  
  VALUE mEnum = rb_define_module_under(mGimp, "EnumNames");
  enum_type_names = gimp_enums_get_type_names (&n_enum_type_names);
  for (i = 0; i < n_enum_type_names; i++)
    {
      const gchar *enum_name  = enum_type_names[i];
      GType        enum_type  = g_type_from_name (enum_name);
      GEnumClass  *enum_class = g_type_class_ref (enum_type);
      GEnumValue  *value;
      
      /* Create and add the new name hash */
      volatile VALUE hash = rb_hash_new();
      rb_define_const(mEnum, enum_name + 4, hash);

      /* Iterate over the enums */
      for (value = enum_class->values; value->value_name; value++)
        {
          gchar *name = value->value_name + 5;
          volatile VALUE rbname = rb_str_new2(name);
          VALUE num = INT2NUM(value->value);
          
          /* Add the enum and name to the name hash */
          rb_hash_aset(hash, num, rbname);
          /* Add the enum to the Gimp module */
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
  
  rb_define_const(mGimp, "GIMP_DIRECTORY", rb_str_new2(gimp_directory()));
  rb_define_const(mGimp, "GIMP_DATA_DIRECTORY", rb_str_new2(gimp_data_directory()));
  rb_define_const(mGimp, "GIMP_PLUG_IN_DIRECTORY", rb_str_new2(gimp_plug_in_directory()));
  rb_define_const(mGimp, "GIMP_LOCALE_DIRECTORY", rb_str_new2(gimp_locale_directory()));
  rb_define_const(mGimp, "GIMP_SYSCONF_DIRECTORY", rb_str_new2(gimp_sysconf_directory()));
}
