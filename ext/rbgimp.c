#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE mGimp;

static VALUE
rb_gimp_version (VALUE self)
{
    return rb_str_new2(gimp_version());
}

void Init_gimpext(void)
{
    mGimp = rb_define_module("Gimp");
    rb_define_module_function(mGimp, "version", rb_gimp_version, 0);
    rb_define_const(mGimp, "MAJOR_VERSION", INT2NUM(gimp_major_version));
    rb_define_const(mGimp, "MINOR_VERSION", INT2NUM(gimp_minor_version));
    rb_define_const(mGimp, "MICRO_VERSION", INT2NUM(gimp_micro_version));

    Init_structures();
    Init_ids();

    Init_gimpconstants();
    Init_gimpmain();
  Init_gimpproceduraldb();

    Init_gimpcolorrgb();
  Init_gimpcolorhsv();
  Init_gimpcolorcmyk();
  Init_gimpcolorspace();
}
