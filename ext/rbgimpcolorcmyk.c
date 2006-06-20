#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE cGimpCMYK;

static VALUE
rb_gimp_cmyk_alloc (VALUE klass)
{
  GimpCMYK *color = ALLOC(GimpCMYK);
  return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE
rb_gimp_cmyk_initialize (int    argc,
                         VALUE *argv,
                         VALUE  self)
{
  volatile VALUE c, m, y, k, a;
  rb_scan_args(argc, argv, "05", &c, &m, &y, &k, &a);

  GimpCMYK *color;
  Data_Get_Struct(self, GimpCMYK, color);

  if(argc == 0)
    {
      color->c = 0.0;
      color->m = 0.0;
      color->y = 0.0;
      color->k = 0.0;
      color->a = 1.0;
    }
  else if(argc == 3)
    {
      color->c = (gdouble)NUM2DBL(c);
      color->m = (gdouble)NUM2DBL(m);
      color->y = (gdouble)NUM2DBL(y);
      color->k = (gdouble)NUM2DBL(k);
      color->a = 1.0;
    }
  else if(argc == 5)
    {
      color->c = (gdouble)NUM2DBL(c);
      color->m = (gdouble)NUM2DBL(m);
      color->y = (gdouble)NUM2DBL(y);
      color->k = (gdouble)NUM2DBL(k);
      color->a = (gdouble)NUM2DBL(a);
    }
  else
    {
      rb_raise(rb_eArgError, "Wrong number of parameters. (0, 4 or 5)");
    }

  return self;
}

static VALUE
rb_gimp_cmyk_get_c (VALUE self)
{
  gdouble result = rb2GimpCMYKPtr(self)->c;
  return rb_float_new(result);
}

static VALUE
rb_gimp_CMYK_set_c (VALUE self,
                    VALUE value)
{
  GimpCMYK *color = rb2GimpCMYKPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->c = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->c = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
        rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_cmyk_get_m (VALUE self)
{
  gdouble result = rb2GimpCMYKPtr(self)->m;
  return rb_float_new(result);
}

static VALUE
rb_gimp_CMYK_set_m (VALUE self,
                    VALUE value)
{
  GimpCMYK *color = rb2GimpCMYKPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->m = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->m = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
        rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_cmyk_get_y (VALUE self)
{
  gdouble result = rb2GimpCMYKPtr(self)->y;
  return rb_float_new(result);
}

static VALUE
rb_gimp_CMYK_set_y (VALUE self,
                    VALUE value)
{
  GimpCMYK *color = rb2GimpCMYKPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->y = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->y = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
        rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_cmyk_get_k (VALUE self)
{
  gdouble result = rb2GimpCMYKPtr(self)->k;
  return rb_float_new(result);
}

static VALUE
rb_gimp_CMYK_set_k(VALUE self,
                   VALUE value)
{
  GimpCMYK *color = rb2GimpCMYKPtr(value);
  if(rb_obj_is_kind_of(value, rb_cFloat))
    {
      color->k = (gdouble)NUM2DBL(value);
    }
  else if(rb_obj_is_kind_of(value, rb_cFixnum))
    {
      color->k = (gdouble)(NUM2DBL(value)/255.0);
    }
  else
    {
        rb_raise(rb_eTypeError, "Argument must be a Float or a Fixnum.");
    }

  return value;
}

static VALUE
rb_gimp_cmyk_get_a (VALUE self)
{
  gdouble result = rb2GimpCMYKPtr(self)->a;
  return rb_float_new(result);
}

static VALUE
rb_gimp_CMYK_set_a (VALUE self,
                    VALUE value)
{
  GimpCMYK *color = rb2GimpCMYKPtr(value);
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
rb_gimp_cmyk_set (VALUE self,
                  VALUE c,
                  VALUE m,
                  VALUE y,
                  VALUE k)
{
  GimpCMYK *color;
  Data_Get_Struct(self, GimpCMYK, color);

  gimp_cmyk_set(color,
                (gdouble)NUM2DBL(c),
                (gdouble)NUM2DBL(m),
                (gdouble)NUM2DBL(y),
                (gdouble)NUM2DBL(k));

  return rb_ary_new3(4, c, m, y, k);
}

static VALUE
rb_gimp_cmyka_set (VALUE self,
                   VALUE c,
                   VALUE m,
                   VALUE y,
                   VALUE k,
                   VALUE a)
{
  GimpCMYK *color;
  Data_Get_Struct(self, GimpCMYK, color);

  gimp_cmyka_set(color,
                 (gdouble)NUM2DBL(c),
                 (gdouble)NUM2DBL(m),
                 (gdouble)NUM2DBL(y),
                 (gdouble)NUM2DBL(k),
                 (gdouble)NUM2DBL(a));

  return rb_ary_new3(5, c, m, y, k);
}

static VALUE
rb_gimp_cmyk_get_uchar (VALUE self)
{
  guchar c, m, y, k;
  gimp_cmyk_get_uchar(rb2GimpCMYKPtr(self), &c, &m, &y, &k);

  return rb_ary_new3(4,
                     INT2NUM(c),
                     INT2NUM(m),
                     INT2NUM(y),
                     INT2NUM(k));
}

static VALUE
rb_gimp_cmyka_get_uchar (VALUE self)
{
  guchar c, m, y, k, a;
  gimp_cmyka_get_uchar(rb2GimpCMYKPtr(self), &c, &m, &y, &k, &a);

  return rb_ary_new3(5,
                     INT2NUM(c),
                     INT2NUM(m),
                     INT2NUM(y),
                     INT2NUM(k),
                     INT2NUM(a));
}

static VALUE
rb_gimp_cmyka_set_uchar (VALUE self,
                         VALUE c,
                         VALUE m,
                         VALUE y,
                         VALUE k,
                         VALUE a)
{
  gimp_cmyka_set_uchar(rb2GimpCMYKPtr(self),
                       (guchar)NUM2INT(c),
                       (guchar)NUM2INT(m),
                       (guchar)NUM2INT(y),
                       (guchar)NUM2INT(k),
                       (guchar)NUM2INT(a));

  return rb_ary_new3(5, c, m, y, k, a);
}

static VALUE
rb_gimp_cmyk_set_uchar (VALUE self,
                        VALUE c,
                        VALUE m,
                        VALUE y,
                        VALUE k)
{
  gimp_cmyk_set_uchar(rb2GimpCMYKPtr(self),
                      (guchar)NUM2INT(c),
                      (guchar)NUM2INT(m),
                      (guchar)NUM2INT(y),
                      (guchar)NUM2INT(k));

  return rb_ary_new3(4, c, m, y, k);
}

void Init_gimpcolorcmyk(void)
{
  cGimpCMYK = rb_define_class_under(mGimp, "Cmyk", rb_cObject);
  rb_define_alloc_func(cGimpCMYK, rb_gimp_cmyk_alloc);
  rb_define_method(cGimpCMYK, "initialize", rb_gimp_cmyk_initialize, -1);

  rb_define_method(cGimpCMYK, "set", rb_gimp_cmyk_set, 3);
  rb_define_method(cGimpCMYK, "cmyka_set", rb_gimp_cmyka_set, 4);

}
