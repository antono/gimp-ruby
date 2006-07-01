#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE cGimpRGB;

static VALUE
rb_gimp_rgb_alloc (VALUE klass)
{
  GimpRGB *color = ALLOC(GimpRGB);
  return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE
rb_gimp_rgb_initialize(int argc, VALUE *argv, VALUE self)
{
  volatile VALUE r, g, b, a;
  rb_scan_args(argc, argv, "04", &r, &g, &b, &a);

  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  if(argc == 0)
    {
      color->r = 0.0;
      color->g = 0.0;
      color->b = 0.0;
      color->a = 1.0;
    }
  else if(argc == 3)
    {
      color->r = (gdouble)NUM2DBL(r);
      color->g = (gdouble)NUM2DBL(g);
      color->b = (gdouble)NUM2DBL(b);
      color->a = 1.0;
    }
  else if(argc == 4)
    {
      color->r = (gdouble)NUM2DBL(r);
      color->g = (gdouble)NUM2DBL(g);
      color->b = (gdouble)NUM2DBL(b);
      color->a = (gdouble)NUM2DBL(a);
    }
  else
    {
      rb_raise(rb_eArgError, "Wrong number of parameters. (0, 3 or 4)");
    }

  return self;
}

static VALUE
rb_gimp_rgb_set (VALUE self,
                 VALUE r,
                 VALUE g,
                 VALUE b)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  gimp_rgb_set(color,
               (gdouble)NUM2DBL(r),
               (gdouble)NUM2DBL(g),
               (gdouble)NUM2DBL(b));

  return rb_ary_new3(3, r, g, b);
}

static VALUE
rb_gimp_rgba_set (VALUE self,
                  VALUE r,
                  VALUE g,
                  VALUE b,
                  VALUE a)
{
    GimpRGB *color;
    Data_Get_Struct(self, GimpRGB, color);

    gimp_rgba_set(color,
                  (gdouble)NUM2DBL(r),
                  (gdouble)NUM2DBL(g),
                  (gdouble)NUM2DBL(b),
                  (gdouble)NUM2DBL(a));

    return rb_ary_new3(4, r, g, b, a);
}

static VALUE
rb_gimp_rgb_get_r (VALUE self)
{
  gdouble result = rb2GimpRGBPtr(self)->r;
  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_set_r (VALUE self,
                   VALUE value)
{
  GimpRGB *color = rb2GimpRGBPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->r = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->r = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
      rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_rgb_get_g (VALUE self)
{
  gdouble result = rb2GimpRGBPtr(self)->g;
  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_set_g (VALUE self,
                   VALUE value)
{
  GimpRGB *color = rb2GimpRGBPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->g = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->g = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
      rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static
VALUE rb_gimp_rgb_get_b(VALUE self)
{
  gdouble result = rb2GimpRGBPtr(self)->b;
  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_set_b(VALUE self, VALUE value)
{
  GimpRGB *color = rb2GimpRGBPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->b = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->b = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
      rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_rgb_get_a (VALUE self)
{
  gdouble result = rb2GimpRGBPtr(self)->a;
  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_set_a (VALUE self,
                   VALUE value)
{
  GimpRGB *color = rb2GimpRGBPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->a = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->a = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
      rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_rgb_set_alpha (VALUE self,
                       VALUE alpha)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  gimp_rgb_set_alpha(color, (gdouble)NUM2DBL(alpha));

  return alpha;
}

static VALUE
rb_gimp_rgb_set_uchar (VALUE self,
                       VALUE r,
                       VALUE g,
                       VALUE b)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  gimp_rgb_set_uchar(color,
                     (guchar)NUM2INT(r),
                     (guchar)NUM2INT(g),
                     (guchar)NUM2INT(b));

  return rb_ary_new3(3, r, g, b);
}

static VALUE
rb_gimp_rgba_set_uchar (VALUE self,
                        VALUE r,
                        VALUE g,
                        VALUE b,
                        VALUE a)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  gimp_rgba_set_uchar(color,
                      (guchar)NUM2INT(r),
                      (guchar)NUM2INT(g),
                      (guchar)NUM2INT(b),
                      (guchar)NUM2INT(a));

  return rb_ary_new3(4, r, g, b, a);
}

static VALUE
rb_gimp_rgb_get_uchar (VALUE self)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  guchar r, g, b;
  gimp_rgb_get_uchar(color, &r, &g, &b);

  return rb_ary_new3(3, INT2FIX(r), INT2FIX(g), INT2FIX(b));
}

static VALUE
rb_gimp_rgba_get_uchar (VALUE self)
{
  GimpRGB *color;
  Data_Get_Struct(self, GimpRGB, color);

  guchar r, g, b, a;
  gimp_rgba_get_uchar(color, &r, &g, &b, &a);

  return rb_ary_new3(4, INT2FIX(r), INT2FIX(g), INT2FIX(b), INT2FIX(a));
}

static VALUE
rb_gimp_rgb_parse_name (VALUE self, VALUE name)
{
  GimpRGB color;
  gboolean success;
  success = gimp_rgb_parse_name(&color,
                                (gchar *)StringValuePtr(name),
                                (gint)-1);

  if(!success)
    return Qnil;

  return GimpRGB2rb(&color);
}

static VALUE
rb_gimp_rgb_parse_hex (VALUE self,
                       VALUE hex)
{
  GimpRGB color;
  gboolean success;
  success = gimp_rgb_parse_hex(&color,
                               (gchar *)StringValuePtr(hex),
                               (gint)-1);

  if(!success)
    return Qnil;

  return GimpRGB2rb(&color);
}

static VALUE
rb_gimp_rgb_parse_css (VALUE self,
                       VALUE css)
{
  GimpRGB color;
  gboolean success;
  success = gimp_rgb_parse_css(&color,
                               (gchar *)StringValuePtr(css),
                               (gint)-1);

  if(!success)
    return Qnil;

  return GimpRGB2rb(&color);
}

static VALUE
rb_gimp_rgba_parse_css (VALUE self,
                        VALUE css)
{
  GimpRGB color;
  gboolean success;
  success = gimp_rgba_parse_css(&color,
                                (gchar *)StringValuePtr(css),
                                (gint)-1);

  if(!success)
    return Qnil;

  return GimpRGB2rb(&color);
}

static VALUE
rb_gimp_rgb_list_names (VALUE self)
{
  const gchar **names;
  GimpRGB *colors;
  gint num = gimp_rgb_list_names(&names, &colors);
  int i;

  volatile VALUE rbnames = rb_ary_new();
  for(i=0; i<num; i++)
    rb_ary_push(rbnames, rb_str_new2(names[i]));

  volatile VALUE rbcolors = rb_ary_new();
  for(i=0; i<num; i++)
    rb_ary_push(rbcolors, GimpRGB2rb(&colors[i]));

  free(names);
  free(colors);

  return rb_ary_new3(2, rbnames, rbcolors);
}

static VALUE
rb_gimp_rgb_add (VALUE self,
                 VALUE other)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgb_add(&result, rb2GimpRGBPtr(other));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgba_add (VALUE self,
                  VALUE other)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgba_add(&result, rb2GimpRGBPtr(other));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgb_subtract (VALUE self,
                      VALUE other)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgb_subtract(&result, rb2GimpRGBPtr(other));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgba_subtract (VALUE self,
                       VALUE other)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgba_subtract(&result, rb2GimpRGBPtr(other));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgb_multiply (VALUE self,
                      VALUE factor)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgb_multiply(&result, (gdouble)NUM2DBL(factor));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgba_multiply (VALUE self,
                       VALUE factor)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgba_multiply(&result, (gdouble)NUM2DBL(factor));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgb_distance (VALUE self,
                      VALUE other)
{
  gdouble result;
  result = gimp_rgb_distance(rb2GimpRGBPtr(self),
                             rb2GimpRGBPtr(other));

  return rb_float_new(result);
}

static VALUE
rb_gimp_rgba_distance (VALUE self,
                       VALUE other)
{
  gdouble result;
  result = gimp_rgba_distance(rb2GimpRGBPtr(self),
                              rb2GimpRGBPtr(other));

  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_max (VALUE self)
{
  gdouble result;
  result = gimp_rgb_max(rb2GimpRGBPtr(self));

  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_min (VALUE self)
{
  gdouble result;
  result = gimp_rgb_min(rb2GimpRGBPtr(self));

  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_clamp (VALUE self)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgb_clamp(&result);

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgb_clamp_replace (VALUE self)
{
  gimp_rgb_clamp(rb2GimpRGBPtr(self));
  return self;
}

static VALUE
rb_gimp_rgb_gamma (VALUE self,
                   VALUE gamma)
{
  GimpRGB result = rb2GimpRGB(self);
  gimp_rgb_gamma(&result, (gdouble)NUM2DBL(gamma));

  return GimpRGB2rb(&result);
}

static VALUE
rb_gimp_rgb_gamma_replace (VALUE self,
                           VALUE gamma)
{
  gimp_rgb_gamma(rb2GimpRGBPtr(self),
                 (gdouble)NUM2DBL(gamma));

  return self;
}

static VALUE
rb_gimp_rgb_luminance (VALUE self)
{
  gdouble result;
  result = gimp_rgb_luminance(rb2GimpRGBPtr(self));

  return rb_float_new(result);
}

static VALUE
rb_gimp_rgb_luminance_uchar (VALUE self)
{
  guchar result;
  result = gimp_rgb_luminance_uchar(rb2GimpRGBPtr(self));

  return FIX2INT(result);
}

static VALUE
rb_gimp_rgb_composite (VALUE self,
                       VALUE other,
                       VALUE mode)
{
  GimpRGB color = rb2GimpRGB(self);
  gimp_rgb_composite(&color,
                     rb2GimpRGBPtr(other),
                     (GimpRGBCompositeMode)NUM2INT(mode));

  return GimpRGB2rb(&color);
}

static VALUE
rb_gimp_rgb_composite_replace (VALUE self,
                               VALUE other,
                               VALUE mode)
{
  gimp_rgb_composite(rb2GimpRGBPtr(self),
                     rb2GimpRGBPtr(other),
                     (GimpRGBCompositeMode)NUM2INT(mode));

  return self;
}

static VALUE
rb_GIMP_RGB_LUMINANCE (VALUE self,
                       VALUE r,
                       VALUE g,
                       VALUE b)
{
  gdouble result;
  result = GIMP_RGB_LUMINANCE((gdouble)NUM2DBL(r),
                              (gdouble)NUM2DBL(g),
                              (gdouble)NUM2DBL(b));

  return rb_float_new(result);
}

static VALUE
rb_GIMP_RGB_INTENSITY (VALUE self,
                       VALUE r,
                       VALUE g,
                       VALUE b)
{
  gdouble result;
  result = GIMP_RGB_INTENSITY((gdouble)NUM2DBL(r),
                              (gdouble)NUM2DBL(g),
                              (gdouble)NUM2DBL(b));

  return rb_float_new(result);
}

/*GIMP_TYPE_RGB
GIMP_TYPE_PARAM_RGB
gimp_param_spec_rgb
GIMP_VALUE_HOLDS_RGB*/

void Init_gimpcolorrgb(void)
{
  cGimpRGB = rb_define_class_under(mGimp, "Rgb", rb_cObject);
  rb_define_alloc_func(cGimpRGB, rb_gimp_rgb_alloc);
  rb_define_method(cGimpRGB, "initialize", rb_gimp_rgb_initialize, -1);

  rb_define_method(cGimpRGB, "set", rb_gimp_rgb_set, 3);
  rb_define_method(cGimpRGB, "rgba_set", rb_gimp_rgba_set, 4);

  rb_define_method(cGimpRGB, "r", rb_gimp_rgb_get_r, 0);
  rb_define_method(cGimpRGB, "g", rb_gimp_rgb_get_g, 0);
  rb_define_method(cGimpRGB, "b", rb_gimp_rgb_get_b, 0);
  rb_define_method(cGimpRGB, "a", rb_gimp_rgb_get_a, 0);

  rb_define_method(cGimpRGB, "r=", rb_gimp_rgb_set_r, 1);
  rb_define_method(cGimpRGB, "g=", rb_gimp_rgb_set_g, 1);
  rb_define_method(cGimpRGB, "b=", rb_gimp_rgb_set_b, 1);
  rb_define_method(cGimpRGB, "a=", rb_gimp_rgb_set_a, 1);

  rb_define_method(cGimpRGB, "set_alpha", rb_gimp_rgb_set_alpha, 1);

  rb_define_method(cGimpRGB, "set_uchar", rb_gimp_rgb_set_uchar, 3);
  rb_define_method(cGimpRGB, "rgba_set_uchar", rb_gimp_rgba_set_uchar, 4);

  rb_define_method(cGimpRGB, "get_uchar", rb_gimp_rgb_get_uchar, 0);
  rb_define_method(cGimpRGB, "rgba_get_uchar", rb_gimp_rgba_get_uchar, 0);

  rb_define_method(cGimpRGB, "parse_name", rb_gimp_rgb_parse_name, 1);
  rb_define_method(cGimpRGB, "parse_hex", rb_gimp_rgb_parse_hex, 1);

  rb_define_method(cGimpRGB, "parse_css", rb_gimp_rgb_parse_css, 1);
  rb_define_method(cGimpRGB, "rgba_parse_css", rb_gimp_rgba_parse_css, 1);

  rb_define_method(cGimpRGB, "list_names", rb_gimp_rgb_list_names, 0);

  rb_define_method(cGimpRGB, "add", rb_gimp_rgb_add, 1);
  rb_define_method(cGimpRGB, "rgba_add", rb_gimp_rgba_add, 1);
  rb_define_alias(cGimpRGB, "+", "add");

  rb_define_method(cGimpRGB, "subtract", rb_gimp_rgb_subtract, 1);
  rb_define_method(cGimpRGB, "rgba_subtract", rb_gimp_rgba_subtract, 1);
  rb_define_alias(cGimpRGB, "-", "subtract");

  rb_define_method(cGimpRGB, "multiply", rb_gimp_rgb_multiply, 1);
  rb_define_method(cGimpRGB, "rgba_multiply", rb_gimp_rgba_multiply, 1);
  rb_define_alias(cGimpRGB, "*", "multiply");

  rb_define_method(cGimpRGB, "distance", rb_gimp_rgb_distance, 0);
  rb_define_method(cGimpRGB, "rgba_distance", rb_gimp_rgba_distance, 0);

  rb_define_method(cGimpRGB, "max", rb_gimp_rgb_max, 0);
  rb_define_method(cGimpRGB, "min", rb_gimp_rgb_min, 0);

  rb_define_method(cGimpRGB, "clamp", rb_gimp_rgb_clamp, 0);
  rb_define_method(cGimpRGB, "clamp!", rb_gimp_rgb_clamp_replace, 0);

  rb_define_method(cGimpRGB, "gamma", rb_gimp_rgb_gamma, 1);
  rb_define_method(cGimpRGB, "gamma!", rb_gimp_rgb_gamma_replace, 0);

  rb_define_method(cGimpRGB, "luminance", rb_gimp_rgb_luminance, 0);
  rb_define_method(cGimpRGB, "luminance_uchar", rb_gimp_rgb_luminance_uchar, 0);

  rb_define_method(cGimpRGB, "composite", rb_gimp_rgb_composite, 2);
  rb_define_method(cGimpRGB, "composite!", rb_gimp_rgb_composite_replace, 0);

  rb_define_singleton_method(cGimpRGB, "LUMINANCE", rb_GIMP_RGB_LUMINANCE, 3);
  rb_define_singleton_method(cGimpRGB, "INTENSITY", rb_GIMP_RGB_INTENSITY, 3);

  rb_define_const(cGimpRGB, "LUMINANCE_RED", GIMP_RGB_LUMINANCE_RED);
  rb_define_const(cGimpRGB, "LUMINANCE_GREEN", GIMP_RGB_LUMINANCE_GREEN);
  rb_define_const(cGimpRGB, "LUMINANCE_BLUE", GIMP_RGB_LUMINANCE_BLUE);

  rb_define_const(cGimpRGB, "INTENSITY_RED", GIMP_RGB_INTENSITY_RED);
  rb_define_const(cGimpRGB, "INTENSITY_GREEN", GIMP_RGB_INTENSITY_GREEN);
  rb_define_const(cGimpRGB, "INTENSITY_BLUE", GIMP_RGB_INTENSITY_BLUE);
}
