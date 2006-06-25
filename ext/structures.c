#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

VALUE sGimpParamDef;
VALUE sGimpParam;
VALUE sGimpPlugInInfo;
VALUE sGimpParamRegion;

void Init_structures(void)
{
    sGimpParamDef = rb_struct_define("GimpParamDef", "type", "name", "dscr", NULL);
    rb_define_const(mGimp, "ParamDef", sGimpParamDef);

    sGimpParam = rb_struct_define("GimpParam", "type", "data", NULL);
    rb_define_const(mGimp, "Param", sGimpParam);

    sGimpPlugInInfo = rb_struct_define("GimpPlugInInfo",
                                       "init_proc",
                                       "quit_proc",
                                       "query_proc",
                                       "run_proc", NULL);
    rb_define_const(mGimp, "PlugInInfo", sGimpPlugInInfo);
    
    sGimpParamRegion = rb_struct_define("GimpParamRegion",
                                        "x",
                                        "y",
                                        "width",
                                        "height");
    rb_define_const(mGimp, "ParamRegion", sGimpParamRegion);
}
