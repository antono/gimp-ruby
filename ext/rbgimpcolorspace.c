#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

static VALUE
rb_gimp_rgb_to_hsv (VALUE self,
                    VALUE rgb)
{
  GimpHSV hsv;
  gimp_rgb_to_hsv(rb2GimpRGBPtr(rgb), &hsv);

  return GimpHSV2rb(&hsv);
}

static VALUE
rb_gimp_rgb_to_hsl (VALUE self,
                    VALUE rgb)
{
  //TODO
  rb_notimplement();
  return Qnil;
}

static VALUE
rb_gimp_rgb_to_cmyk (VALUE self,
                     VALUE rgb,
                     VALUE pullout)
{
  GimpCMYK cmyk;
  gimp_rgb_to_cmyk(rb2GimpRGBPtr(rgb),
                   (gdouble)NUM2DBL(pullout),
                   &cmyk);

  return GimpCMYK2rb(&cmyk);
}

static VALUE
rb_gimp_hsv_to_rgb (VALUE self,
                    VALUE hsv)
{
  GimpRGB rgb;
  gimp_hsv_to_rgb(rb2GimpHSVPtr(hsv), &rgb);

  return GimpRGB2rb(&rgb);
}

static VALUE
rb_gimp_hsl_to_rgb (VALUE self,
                    VALUE hsl)
{
  //TODO
  rb_notimplement();
  return Qnil;
}

static VALUE
rb_gimp_cmyk_to_rgb (VALUE self,
                     VALUE cmyk)
{
  GimpRGB rgb;
  gimp_cmyk_to_rgb(rb2GimpCMYKPtr(cmyk), &rgb);//rb2CMYKPtr(cmyk), &rgb);

  return GimpRGB2rb(&rgb);
}

static VALUE
rb_gimp_rgb_to_hwb (VALUE self,
                    VALUE rgb)
{
  gdouble hue, whiteness, blackness;
  gimp_rgb_to_hwb(rb2GimpRGBPtr(rgb),
                  &hue,
                  &whiteness,
                  &blackness);

  return rb_ary_new3(3,
                     rb_float_new(hue),
                     rb_float_new(whiteness),
                     rb_float_new(blackness));
}

static VALUE
rb_gimp_hwb_to_rgb (VALUE self,
                    VALUE hue,
                    VALUE whiteness,
                    VALUE blackness)
{
  GimpRGB rgb;
  gimp_hwb_to_rgb((gdouble)NUM2DBL(hue),
                  (gdouble)NUM2DBL(whiteness),
                  (gdouble)NUM2DBL(blackness),
                  &rgb);

  return GimpRGB2rb(&rgb);
}

static VALUE
rb_gimp_rgb_to_hsv_int (VALUE self,
                        VALUE rbred,
                        VALUE rbgreen,
                        VALUE rbblue)
{
  gint red = (gint)NUM2INT(rbred);
  gint green = (gint)NUM2INT(rbgreen);
  gint blue = (gint)NUM2INT(rbblue);

  gimp_rgb_to_hsv_int(&red, &green, &blue);

  return rb_ary_new3(3,
                     INT2NUM(red),
                     INT2NUM(green),
                     INT2NUM(blue));
}

static VALUE
rb_gimp_hsv_to_rgb_int (VALUE self,
                   VALUE rbhue,
                   VALUE rbsaturation,
                   VALUE rbvalue)
{
  gint hue = (gint)NUM2INT(rbhue);
  gint saturation = (gint)NUM2INT(rbsaturation);
  gint value = (gint)NUM2INT(rbvalue);

  gimp_hsv_to_rgb_int(&hue, &saturation, &value);

  return rb_ary_new3(3,
                     INT2NUM(hue),
                     INT2NUM(saturation),
                     INT2NUM(value));
}

static VALUE
rb_gimp_rgb_to_hsl_int (VALUE self,
                   VALUE rbred,
                   VALUE rbgreen,
                   VALUE rbblue)
{
  gint red = (gint)NUM2INT(rbred);
  gint green = (gint)NUM2INT(rbgreen);
  gint blue = (gint)NUM2INT(rbblue);

  gimp_rgb_to_hsl_int(&red, &green, &blue);

  return rb_ary_new3(3,
                     INT2NUM(red),
                     INT2NUM(green),
                     INT2NUM(blue));
}

static VALUE
rb_gimp_hsl_to_rgb_int (VALUE self,
                        VALUE rbhue,
                        VALUE rbsaturation,
                        VALUE rblightness)
{
  gint hue = (gint)NUM2INT(rbhue);
  gint saturation = (gint)NUM2INT(rbsaturation);
  gint lightness = (gint)NUM2INT(rblightness);

  gimp_hsl_to_rgb_int(&hue, &saturation, &lightness);

  return rb_ary_new3(3,
                     INT2NUM(hue),
                     INT2NUM(saturation),
                     INT2NUM(lightness));
}

static VALUE
rb_gimp_rgb_to_cmyk_int (VALUE self,
                         VALUE rbred,
                         VALUE rbgreen,
                         VALUE rbblue,
                         VALUE rbpullout)
{
  gint red = (gint)NUM2INT(red);
  gint green = (gint)NUM2INT(green);
  gint blue = (gint)NUM2INT(blue);
  gint pullout = (gint)NUM2INT(pullout);

  gimp_rgb_to_cmyk_int(&red, &green, &blue, &pullout);

  return rb_ary_new3(4,
                     INT2NUM(red),
                     INT2NUM(green),
                     INT2NUM(blue),
                     INT2NUM(pullout));
}

static VALUE
rb_gimp_cmyk_to_rgb_int (VALUE self,
                         VALUE rbcyan,
                         VALUE rbmagenta,
                         VALUE rbyellow,
                         VALUE rbblack)
{
  gint cyan = (gint)NUM2INT(rbcyan);
  gint magenta = (gint)NUM2INT(rbmagenta);
  gint yellow = (gint)NUM2INT(rbyellow);
  gint black = (gint)NUM2INT(rbblack);

  gimp_cmyk_to_rgb_int(&cyan, &magenta, &yellow, &black);

  return rb_ary_new3(3,
                     INT2NUM(cyan),
                     INT2NUM(magenta),
                     INT2NUM(yellow));
}

static VALUE
rb_gimp_rgb_to_l_int (VALUE self,
                      VALUE red,
                      VALUE green,
                      VALUE blue)
{
  gint lightness = gimp_rgb_to_l_int(NUM2INT(red),
                                     NUM2INT(green),
                                     NUM2INT(blue));

  return INT2NUM(lightness);
}

void Init_gimpcolorspace(void)
{
  rb_define_module_function(mGimp, "rgb_to_hsv", rb_gimp_rgb_to_hsv, 1);
  rb_define_module_function(mGimp, "rgb_to_hsl", rb_gimp_rgb_to_hsl, 1);
  rb_define_module_function(mGimp, "rgb_to_cmyk", rb_gimp_rgb_to_cmyk, 1);

  rb_define_module_function(mGimp, "hsv_to_rgb", rb_gimp_hsv_to_rgb, 1);
  rb_define_module_function(mGimp, "hsl_to_rgb", rb_gimp_hsl_to_rgb, 1);
  rb_define_module_function(mGimp, "cmyk_to_rgb", rb_gimp_cmyk_to_rgb, 1);

  rb_define_module_function(mGimp, "rgb_to_hwb", rb_gimp_rgb_to_hwb, 3);
  rb_define_module_function(mGimp, "hwb_to_rgb", rb_gimp_hwb_to_rgb, 3);

  rb_define_module_function(mGimp, "rgb_to_hsv_int", rb_gimp_rgb_to_hsv_int, 3);
  rb_define_module_function(mGimp, "hsv_to_rgb_int", rb_gimp_hsv_to_rgb_int, 3);

  rb_define_module_function(mGimp, "rgb_to_hsl_int", rb_gimp_rgb_to_hsl_int, 3);
  rb_define_module_function(mGimp, "hsl_to_rgb_int", rb_gimp_hsl_to_rgb_int, 3);

  rb_define_module_function(mGimp, "rgb_to_cmyk_int", rb_gimp_rgb_to_cmyk_int, 4);
  rb_define_module_function(mGimp, "cmyk_to_rgb_int", rb_gimp_cmyk_to_rgb_int, 4);

  rb_define_module_function(mGimp, "rgb_to_l_int", rb_gimp_rgb_to_l_int, 3);
}
