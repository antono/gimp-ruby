#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE cGimpHSV;

static VALUE
rb_gimp_hsv_alloc (VALUE klass)
{
  GimpHSV *color = ALLOC(GimpHSV);
  return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE
rb_gimp_hsv_initialize (int    argc,
                        VALUE *argv,
                        VALUE  self)
{
  volatile VALUE h, s, v, a;
  rb_scan_args(argc, argv, "04", &h, &s, &v, &a);

  GimpHSV *color;
  Data_Get_Struct(self, GimpHSV, color);

  if(argc == 0)
    {
      color->h = 0.0;
      color->s = 0.0;
      color->v = 0.0;
      color->a = 1.0;
    }
  else if(argc == 3)
    {
      color->h = (gdouble)NUM2DBL(h);
      color->s = (gdouble)NUM2DBL(s);
      color->v = (gdouble)NUM2DBL(v);
      color->a = 1.0;
    }
  else if(argc == 4)
    {
      color->h = (gdouble)NUM2DBL(h);
      color->s = (gdouble)NUM2DBL(s);
      color->v = (gdouble)NUM2DBL(v);
      color->a = (gdouble)NUM2DBL(a);
    }
  else
    {
      rb_raise(rb_eArgError, "Wrong number of parameters. (0, 3 or 4)");
    }

  return self;
}

static VALUE
rb_gimp_hsv_set (VALUE self,
                 VALUE h,
                 VALUE s,
                 VALUE v)
{
  GimpHSV *color;
  Data_Get_Struct(self, GimpHSV, color);

  gimp_hsv_set(color,
               (gdouble)NUM2DBL(h),
               (gdouble)NUM2DBL(s),
               (gdouble)NUM2DBL(v));

  return rb_ary_new3(3, h, s, v);
}

static VALUE
rb_gimp_hsva_set (VALUE self,
                  VALUE h,
                  VALUE s,
                  VALUE v,
                  VALUE a)
{
  GimpHSV *color;
  Data_Get_Struct(self, GimpHSV, color);

  gimp_hsva_set(color,
                (gdouble)NUM2DBL(h),
                (gdouble)NUM2DBL(s),
                (gdouble)NUM2DBL(v),
                (gdouble)NUM2DBL(a));

  return rb_ary_new3(4, h, s, v, a);
}

static VALUE
rb_gimp_hsv_get_h (VALUE self)
{
  gdouble result = rb2GimpHSVPtr(self)->h;
  return rb_float_new(result);
}

static VALUE
rb_gimp_hsv_set_h (VALUE self,
                   VALUE value)
{
  GimpHSV *color = rb2GimpHSVPtr(value);
  color->h = (gdouble)(NUM2DBL(value)/255.0);

  return value;
}

static VALUE
rb_gimp_hsv_get_s (VALUE self)
{
  gdouble result = rb2GimpHSVPtr(self)->s;
  return rb_float_new(result);
}

static VALUE
rb_gimp_hsv_set_s (VALUE self,
                   VALUE value)
{
  GimpHSV *color = rb2GimpHSVPtr(value);
  color->s = (gdouble)(NUM2DBL(value)/255.0);

  return value;
}

static VALUE
rb_gimp_hsv_get_v (VALUE self)
{
  gdouble result = rb2GimpHSVPtr(self)->v;
  return rb_float_new(result);
}

static VALUE
rb_gimp_hsv_set_v (VALUE self,
                   VALUE value)
{
  GimpHSV *color = rb2GimpHSVPtr(value);
  color->v = (gdouble)(NUM2DBL(value)/255.0);

  return value;
}

static VALUE
rb_gimp_hsv_get_a (VALUE self)
{
  gdouble result = rb2GimpHSVPtr(self)->a;
  return rb_float_new(result);
}

static VALUE
rb_gimp_hsv_set_a(VALUE self, VALUE value)
{
  GimpHSV *color = rb2GimpHSVPtr(value);
  color->a = (gdouble)(NUM2DBL(value)/255.0);

  return value;
}

static VALUE
rb_gimp_hsv_clamp (VALUE self)
{
  GimpHSV result = rb2GimpHSV(self);
  gimp_hsv_clamp(&result);

  return GimpHSV2rb(&result);
}

static VALUE
rb_gimp_hsv_clamp_replace (VALUE self)
{
  gimp_hsv_clamp(rb2GimpHSVPtr(self));
  return self;
}

void Init_gimpcolorhsv(void)
{
  cGimpHSV = rb_define_class_under(mGimp, "Hsv", rb_cObject);
  rb_define_alloc_func(cGimpHSV, rb_gimp_hsv_alloc);
  rb_define_method(cGimpHSV, "initialize", rb_gimp_hsv_initialize, -1);

  rb_define_method(cGimpHSV, "set", rb_gimp_hsv_set, 3);
  rb_define_method(cGimpHSV, "hsva_set", rb_gimp_hsva_set, 4);

  rb_define_method(cGimpHSV, "h", rb_gimp_hsv_get_h, 0);
  rb_define_method(cGimpHSV, "s", rb_gimp_hsv_get_s, 0);
  rb_define_method(cGimpHSV, "v", rb_gimp_hsv_get_v, 0);
  rb_define_method(cGimpHSV, "a", rb_gimp_hsv_get_a, 0);

  rb_define_method(cGimpHSV, "h=", rb_gimp_hsv_set_h, 1);
  rb_define_method(cGimpHSV, "s=", rb_gimp_hsv_set_s, 1);
  rb_define_method(cGimpHSV, "v=", rb_gimp_hsv_set_v, 1);
  rb_define_method(cGimpHSV, "a=", rb_gimp_hsv_set_a, 1);

  rb_define_method(cGimpHSV, "clamp", rb_gimp_hsv_clamp, 0);
  rb_define_method(cGimpHSV, "clamp!", rb_gimp_hsv_clamp_replace, 0);
}
