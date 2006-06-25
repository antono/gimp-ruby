//modules
extern VALUE mGimp;


//classes
extern VALUE cGimpRGB;
extern VALUE cGimpHSV;
extern VALUE cGimpCMYK;
extern VALUE cInt32ArrayStub;
extern VALUE cInt16ArrayStub;
extern VALUE cInt8ArrayStub;
extern VALUE cFloatArrayStub;
extern VALUE cStringArrayStub;


//structures (structures.c)
extern VALUE sGimpParamDef;
extern VALUE sGimpParam;
extern VALUE sGimpPlugInInfo;
extern VALUE sGimpParamRegion;


//Type conversion functions (conversion.c)
VALUE       GimpParam2rb   (GimpParam param);
VALUE       GimpParams2rb  (const GimpParam *params,
                            int              count);
GimpParam   rb2GimpParam   (VALUE            rbparam);
GimpParam  *rb2GimpParams  (VALUE            rbparams,
                            gint            *count);

VALUE         GimpParamDef2rb    (GimpParamDef  paramdef);
VALUE         GimpParamDefs2rb   (GimpParamDef *paramdefs,
                                  int           count);
GimpParamDef  rb2GimpParamDef    (VALUE         param);
GimpParamDef  *rb2GimpParamDefs  (VALUE         rbparamdefs,
                                  int          *count);

VALUE           GimpParamRegion2rb (GimpParamRegion *region);
GimpParamRegion rb2GimpParamRegion (VALUE region);

VALUE    GimpRGB2rb     (GimpRGB *color);
GimpRGB *rb2GimpRGBPtr  (VALUE    color);
GimpRGB  rb2GimpRGB     (VALUE    color);

VALUE    GimpHSV2rb     (GimpHSV *color);
GimpHSV *rb2GimpHSVPtr  (VALUE    color);
GimpHSV  rb2GimpHSV     (VALUE    color);

VALUE     GimpCMYK2rb     (GimpCMYK *color);
GimpCMYK *rb2GimpCMYKPtr  (VALUE     color);
GimpCMYK  rb2GimpCMYK     (VALUE     color);


//ids (ids.c)
extern ID id_type;
extern ID id_name;
extern ID id_dscr;
extern ID id_data;
extern ID id_init_proc;
extern ID id_quit_proc;
extern ID id_query_proc;
extern ID id_run_proc;
extern ID id_call;
extern ID id_x;
extern ID id_y;
extern ID id_width;
extern ID id_height;


//Init functions
void Init_structures(void);
void Init_ids(void);
void Init_arraytypes(void);

void Init_gimpconstants(void);
void Init_gimpmain(void);
void Init_gimpproceduraldb(void);

void Init_gimpcolorrgb(void);
void Init_gimpcolorhsv(void);
void Init_gimpcolorcmyk(void);
void Init_gimpcolorspace(void);
