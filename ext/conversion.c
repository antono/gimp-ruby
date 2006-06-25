#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE
GimpParam2rb (GimpParam param)
{
  GimpPDBArgType type = param.type;
  volatile VALUE data;
  
  switch(type)
    {
    case GIMP_PDB_INT32:
      data = INT2NUM(param.data.d_int32);
      break;
      
    case GIMP_PDB_INT16:
      data = INT2NUM(param.data.d_int16);
      break;
      
    case GIMP_PDB_INT8:
      data = INT2NUM(param.data.d_int8);
      break;
      
    case GIMP_PDB_FLOAT:
      data = rb_float_new(param.data.d_float);
      break;
      
    case GIMP_PDB_STRING:
      data = rb_str_new2(param.data.d_string);
      break;

    case GIMP_PDB_INT32ARRAY:
      data = Data_Wrap_Struct(cInt32ArrayStub, NULL, NULL, param.data.d_int32array);
      break;

    case GIMP_PDB_INT16ARRAY:
      data = Data_Wrap_Struct(cInt16ArrayStub, NULL, NULL, param.data.d_int16array);
      break;

    case GIMP_PDB_INT8ARRAY:
      data = Data_Wrap_Struct(cInt8ArrayStub, NULL, NULL, param.data.d_int8array);
      break;

    case GIMP_PDB_FLOATARRAY:
      data = Data_Wrap_Struct(cFloatArrayStub, NULL, NULL, param.data.d_floatarray);
      break;

    case GIMP_PDB_STRINGARRAY:
      data = Data_Wrap_Struct(cStringArrayStub, NULL, NULL, param.data.d_stringarray);
      break;

    case GIMP_PDB_COLOR:
      data = GimpRGB2rb(&param.data.d_color);
      break;

    case GIMP_PDB_REGION:
      data = GimpParamRegion2rb(&param.data.d_region);
      break;

    case GIMP_PDB_DISPLAY:
      data = INT2NUM(param.data.d_display);
      break;
      
    case GIMP_PDB_IMAGE:
      data = INT2NUM(param.data.d_image);
      break;
      
    case GIMP_PDB_LAYER:
      data = INT2NUM(param.data.d_layer);
      break;
      
//    case GIMP_PDB_LAYER_MASK:
//      data = INT2NUM(param.data.d_layer_mask);
//      break;
      
    case GIMP_PDB_CHANNEL:
      data = INT2NUM(param.data.d_channel);
      break;
      
    case GIMP_PDB_DRAWABLE:
      data = INT2NUM(param.data.d_drawable);
      break;
      
    case GIMP_PDB_SELECTION:
      data = INT2NUM(param.data.d_selection);
      break;
      
    case GIMP_PDB_BOUNDARY:
      data = INT2NUM(param.data.d_boundary);
      break;
      
    case GIMP_PDB_VECTORS:
      data = INT2NUM(param.data.d_vectors);
      break;
      
//    case GIMP_PDB_UNIT:
//      data = INT2NUM(param.data.d_unit);
//      break;
      
    case GIMP_PDB_PARASITE:
      rb_warn("Parasite type is not implemented, sending nil");
      data = Qnil;
      break;

//    case GIMP_PDB_TATTOO:
//      data = INT2NUM(param.data.d_tattoo);
//      break;
      
    case GIMP_PDB_STATUS:
      data = INT2NUM(param.data.d_status);
      break;
    
    default:
      rb_bug("Parameter type %d does not exist.\n", param.type);
    }
    
    return rb_struct_new(sGimpParam, INT2NUM(type), data);
}

VALUE
GimpParams2rb (const GimpParam *params,
               int             count)
{ 
  volatile VALUE ary = rb_ary_new();

  int i;
  for(i=0; i<count; i++)
    rb_ary_push(ary, GimpParam2rb(params[i]));
  
  return ary;
}

static gchar*
rbcopystr(VALUE rbstr)
{
  char *str = StringValuePtr(rbstr);
  gchar *result = g_malloc(strlen(str) + 1);
  strcpy(result, str);
  
  return str;
}

static gpointer
convert_array(GimpPDBArgType type,
              VALUE          rbarr)
{
  rbarr = rb_check_array_type(rbarr);
  int count = RARRAY(rbarr)->len;
  VALUE *arr = RARRAY(rbarr)->ptr;
  
  int i;
  gint32  *int32arr;
  gint16  *int16arr;
  gint8   *int8arr;
  gdouble *floatarr;
  gchar  **stringarr;

  switch(type)
    {
    case GIMP_PDB_INT32ARRAY:
      int32arr = g_new(gint32, count);
      for(i=0; i<count; i++) int32arr[i] = (gint32)NUM2INT(arr[i]);
      return int32arr;
      
    case GIMP_PDB_INT16ARRAY:
      int16arr = g_new(gint16, count);
      for(i=0; i<count; i++) int16arr[i] = (gint16)NUM2INT(arr[i]);
      return int16arr;
      
    case GIMP_PDB_INT8ARRAY:
      int8arr = g_new(gint8, count);
      for(i=0; i<count; i++) int8arr[i] = (gint8)NUM2INT(arr[i]);
      return int8arr;
      
    case GIMP_PDB_FLOATARRAY:
      floatarr = g_new(gdouble, count);
      for(i=0; i<count; i++) floatarr[i] = (gdouble)NUM2DBL(arr[i]);
      return floatarr;
      
    case GIMP_PDB_STRINGARRAY:
      stringarr = g_new(gchar *, count);
      for(i=0; i<count; i++) stringarr[i] = rbcopystr(arr[i]);
      return stringarr;
    default:
      rb_bug("Bad type value in convert_array()");
    }
}

GimpParam
rb2GimpParam (VALUE rbparam)
{
  if(!rb_obj_is_kind_of(rbparam, sGimpParam))
    rb_raise(rb_eTypeError, "Parameter is not a Gimp::Param");

  int type = NUM2INT(rb_struct_aref(rbparam, ID2SYM(id_type)));
  VALUE rbdata = rb_struct_aref(rbparam, ID2SYM(id_data));
  
  GimpParam result;
  result.type = (GimpPDBArgType)type;
  
  switch(type)
    {
    case GIMP_PDB_INT32:
      result.data.d_int32 = (gint32)NUM2INT(rbdata);
      break;
      
    case GIMP_PDB_INT16:
      result.data.d_int16 = (gint16)NUM2INT(rbdata);
      break;
      
    case GIMP_PDB_INT8:
      result.data.d_int8 = (gint8)NUM2INT(rbdata);
      break;
      
    case GIMP_PDB_FLOAT:
      result.data.d_float = (gdouble)NUM2DBL(rbdata);
      break;

    case GIMP_PDB_STRING:
      result.data.d_string = rbcopystr(rbdata);
      break;

    case GIMP_PDB_INT32ARRAY:
      result.data.d_int32array = convert_array(type, rbdata);
      break;

    case GIMP_PDB_INT16ARRAY:
      result.data.d_int16array = convert_array(type, rbdata);
      break;

    case GIMP_PDB_INT8ARRAY:
      result.data.d_int8array = convert_array(type, rbdata);
      break;

    case GIMP_PDB_FLOATARRAY:
      result.data.d_floatarray = convert_array(type, rbdata);
      break;

    case GIMP_PDB_STRINGARRAY:
      result.data.d_stringarray = convert_array(type, rbdata);
      break;

    case GIMP_PDB_COLOR:
      result.data.d_color = rb2GimpRGB(rbdata);
      break;

    case GIMP_PDB_REGION:
      result.data.d_region = rb2GimpParamRegion(rbdata);
      break;

    case GIMP_PDB_DISPLAY:
      result.data.d_display = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_IMAGE:
      result.data.d_image = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_LAYER:
      result.data.d_layer = (gint32)NUM2INT(rbdata);
      break;

//    case GIMP_PDB_LAYER_MASK:
//      result.data.d_layer_mask = (gint32)NUM2INT(rbdata);
//      break;

    case GIMP_PDB_CHANNEL:
      result.data.d_channel = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_DRAWABLE:
      result.data.d_drawable = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_SELECTION:
      result.data.d_selection = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_BOUNDARY:
      result.data.d_boundary = (gint32)NUM2INT(rbdata);
      break;

    case GIMP_PDB_VECTORS:
      result.data.d_vectors = (gint32)NUM2INT(rbdata);
      break;

//    case GIMP_PDB_UNIT:
//      result.data.d_unit = (gint32)NUM2INT(rbdata);
//      break;

    case GIMP_PDB_PARASITE:
      rb_warn("Parasite type is not implemented.");
      rb_notimplement();
      break;

//    case GIMP_PDB_TATTOO:
//      result.data.d_tattoo = (gint32)NUM2INT(rbdata);
//      break;

    case GIMP_PDB_STATUS:
      result.data.d_status = (GimpPDBStatusType)NUM2INT(rbdata);
      break;
    }
  
  return result;
}

GimpParam *
rb2GimpParams (VALUE rbparams,
               gint  *count)
{
  rbparams = rb_check_array_type(rbparams);
  int num = RARRAY(rbparams)->len;
  VALUE *arr = RARRAY(rbparams)->ptr;
  
  GimpParam *params = calloc(num, sizeof(GimpParam));
  
  int i;
  for(i=0; i<num; i++)
    params[i] = rb2GimpParam(arr[i]);
  
  *count = (gint)num;
  return params;
}

VALUE
GimpParamDef2rb (GimpParamDef paramdef)
{
  volatile VALUE type = INT2NUM(paramdef.type);
  volatile VALUE name = rb_str_new2(paramdef.name);
  volatile VALUE desc = rb_str_new2(paramdef.description);
  
  return rb_struct_new(sGimpParamDef, type, name, desc, NULL);
}

VALUE
GimpParamDefs2rb(GimpParamDef *paramdefs,
                 int          count)
{
  volatile VALUE rbparamdefs = rb_ary_new();
  
  int i;
  for(i=0; i<count; i++)
    rb_ary_push(rbparamdefs, GimpParamDef2rb(paramdefs[i]));

  return rbparamdefs;
}

GimpParamDef
rb2GimpParamDef (VALUE param)
{
	if(!rb_obj_is_kind_of(param, sGimpParamDef))
		rb_raise(rb_eArgError, "Parameters must be of type Gimp::ParamDef");
	
	VALUE type = rb_struct_aref(param, ID2SYM(id_type));
	VALUE name = rb_struct_aref(param, ID2SYM(id_name));
	VALUE dscr = rb_struct_aref(param, ID2SYM(id_dscr));
	
	GimpParamDef result;
	result.type = NUM2INT(type);
	result.name = StringValuePtr(name);
	result.description = StringValuePtr(dscr);
	
//	printf("%d %s %s\n", result.type, result.name, result.description);
	return result;
}

GimpParamDef *
rb2GimpParamDefs (VALUE rbparamdefs,
                  gint  *count)
{
  if(rbparamdefs == Qnil)
    {
      *count = 0;
      return NULL;
    }
  else
    {
    	Check_Type(rbparamdefs, T_ARRAY);
  	
      int num = RARRAY(rbparamdefs)->len;
      VALUE *arr = RARRAY(rbparamdefs)->ptr;
    
      GimpParamDef *gimpparamdefs = calloc(num, sizeof(GimpParamDef));
    
      int i;
      for(i=0; i<num; i++)
        gimpparamdefs[i] = rb2GimpParamDef(arr[i]);
      
      *count = (gint)num;
      return gimpparamdefs;
    }
}

VALUE
GimpParamRegion2rb (GimpParamRegion *region)
{
  volatile VALUE x = INT2NUM(region->x);
  volatile VALUE y = INT2NUM(region->y);
  volatile VALUE w = INT2NUM(region->width);
  volatile VALUE h = INT2NUM(region->height);
  
  return rb_struct_new(sGimpParamRegion, x, y, w, h, NULL);
}

GimpParamRegion
rb2GimpParamRegion(VALUE region)
{
	if(!rb_obj_is_kind_of(region, sGimpParamRegion))
		rb_raise(rb_eArgError, "Parameters must be of type Gimp::ParamRegion");
	
	VALUE x = rb_struct_aref(region, ID2SYM(id_x));
	VALUE y = rb_struct_aref(region, ID2SYM(id_y));
	VALUE width = rb_struct_aref(region, ID2SYM(id_width));
	VALUE height = rb_struct_aref(region, ID2SYM(id_height));
	
	GimpParamRegion result;
	result.x = NUM2INT(x);
	result.y = NUM2INT(y);
	result.width = NUM2INT(width);
	result.height = NUM2INT(height);

  return result;
}  

VALUE
GimpRGB2rb (GimpRGB *color)
{
  GimpRGB *tmp = malloc(sizeof(GimpRGB));
  *tmp = *color;
  return Data_Wrap_Struct(cGimpRGB, NULL, free, tmp);
}

GimpRGB *
rb2GimpRGBPtr (VALUE color)
{
  if(!rb_obj_is_kind_of(color, cGimpRGB))
   rb_raise(rb_eTypeError, "Object %s is not a Gimp::RGB", StringValuePtr(color));
  
  GimpRGB *tmp;
  Data_Get_Struct(color, GimpRGB, tmp);
  return tmp;
}

GimpRGB
rb2GimpRGB (VALUE color)
{
  return *rb2GimpRGBPtr(color);
}

VALUE
GimpHSV2rb (GimpHSV *color)
{
  GimpHSV *tmp = ALLOC(GimpHSV);
  *tmp = *color;
  return Data_Wrap_Struct(cGimpHSV, NULL, free, tmp);
}

GimpHSV *
rb2GimpHSVPtr (VALUE color)
{
  if(!rb_obj_is_kind_of(color, cGimpHSV))
   rb_raise(rb_eTypeError, "Object %s is not a Gimp::HSV", StringValuePtr(color));
  
  GimpHSV *tmp;
  Data_Get_Struct(color, GimpHSV, tmp);
  return tmp;
}

GimpHSV
rb2GimpHSV (VALUE color)
{
  return *rb2GimpHSVPtr(color);
}

VALUE
GimpCMYK2rb (GimpCMYK *color)
{
  GimpCMYK *tmp = ALLOC(GimpCMYK);
  *tmp = *color;
  return Data_Wrap_Struct(cGimpCMYK, NULL, free, tmp);
}

GimpCMYK *
rb2GimpCMYKPtr (VALUE color)
{
  if(!rb_obj_is_kind_of(color, cGimpCMYK))
   rb_raise(rb_eTypeError, "Object %s is not a Gimp::CMYK", StringValuePtr(color));

  GimpCMYK *tmp;
  Data_Get_Struct(color, GimpCMYK, tmp);
  return tmp;
}

GimpCMYK
rb2GimpCMYK (VALUE color)
{
  return *rb2GimpCMYKPtr(color);
}
