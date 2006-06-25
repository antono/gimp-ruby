#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE cInt32ArrayStub;
VALUE cInt16ArrayStub;
VALUE cInt8ArrayStub;
VALUE cFloatArrayStub;
VALUE cStringArrayStub;

static VALUE
rb_int32_array_stub_to_a (VALUE self,
                          VALUE rbnum)
{
 gint32 *arr;
 Data_Get_Struct(self, gint32, arr);
 
 int num = NUM2INT(rbnum);
 
 volatile VALUE result = rb_ary_new();
 int i;
 for(i=0; i<num; i++)
   rb_ary_push(result, INT2NUM(arr[i]));
   
  return result;
}

static VALUE
rb_int16_array_stub_to_a (VALUE self,
                          VALUE rbnum)
{
 gint16 *arr;
 Data_Get_Struct(self, gint16, arr);
 
 int num = NUM2INT(rbnum);
 
 volatile VALUE result = rb_ary_new();
 int i;
 for(i=0; i<num; i++)
   rb_ary_push(result, INT2NUM(arr[i]));
   
  return result;
}

static VALUE
rb_int8_array_stub_to_a (VALUE self,
                          VALUE rbnum)
{
 gint8 *arr;
 Data_Get_Struct(self, gint8, arr);
 
 int num = NUM2INT(rbnum);
 
 volatile VALUE result = rb_ary_new();
 int i;
 for(i=0; i<num; i++)
   rb_ary_push(result, INT2NUM(arr[i]));
   
  return result;
}

static VALUE
rb_float_array_stub_to_a (VALUE self,
                          VALUE rbnum)
{
 gfloat *arr;
 Data_Get_Struct(self, gfloat, arr);
 
 int num = NUM2INT(rbnum);
 
 volatile VALUE result = rb_ary_new();
 int i;
 for(i=0; i<num; i++)
   rb_ary_push(result, rb_float_new(arr[i]));
   
  return result;
}

static VALUE
rb_string_array_stub_to_a (VALUE self,
                           VALUE rbnum)
{
 gchar **arr;
 Data_Get_Struct(self, gchar*, arr);
 
 int num = NUM2INT(rbnum);
 
 volatile VALUE result = rb_ary_new();
 int i;
 for(i=0; i<num; i++)
   rb_ary_push(result, rb_str_new2(arr[i]));
   
  return result;
}

void Init_arraytypes(void)
{
  cInt32ArrayStub = rb_define_class_under(mGimp, "Int32ArrayStub", rb_cObject);
  rb_define_method(cInt32ArrayStub, "to_a", rb_int32_array_stub_to_a, 1);

  cInt16ArrayStub = rb_define_class_under(mGimp, "Int16ArrayStub", rb_cObject);
  rb_define_method(cInt16ArrayStub, "to_a", rb_int16_array_stub_to_a, 1);

  cInt8ArrayStub = rb_define_class_under(mGimp, "Int8ArrayStub", rb_cObject);
  rb_define_method(cInt8ArrayStub, "to_a", rb_int8_array_stub_to_a, 1);

  cFloatArrayStub = rb_define_class_under(mGimp, "FloatArrayStub", rb_cObject);
  rb_define_method(cFloatArrayStub, "to_a", rb_float_array_stub_to_a, 1);

  cStringArrayStub = rb_define_class_under(mGimp, "StringArrayStub", rb_cObject);
  rb_define_method(cStringArrayStub, "to_a", rb_string_array_stub_to_a, 1);
}
