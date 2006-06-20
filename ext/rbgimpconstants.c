#include <ruby.h>
#include <libgimp/gimp.h>

#include "rbgimp.h"

void Init_gimpconstants(void)
{
    //GimpAddMaskType
    rb_define_const(mGimp, "ADD_WHITE_MASK", INT2NUM(GIMP_ADD_WHITE_MASK));
    rb_define_const(mGimp, "ADD_BLACK_MASK", INT2NUM(GIMP_ADD_BLACK_MASK));
    rb_define_const(mGimp, "ADD_ALPHA_MASK", INT2NUM(GIMP_ADD_ALPHA_MASK));
    rb_define_const(mGimp, "ADD_ALPHA_TRANSFER_MASK", INT2NUM(GIMP_ADD_ALPHA_TRANSFER_MASK));
    rb_define_const(mGimp, "ADD_SELECTION_MASK", INT2NUM(GIMP_ADD_SELECTION_MASK));
    rb_define_const(mGimp, "ADD_COPY_MASK", INT2NUM(GIMP_ADD_COPY_MASK));

    //GimpBlendMode
    rb_define_const(mGimp, "FG_BG_RGB_MODE", INT2NUM(GIMP_FG_BG_RGB_MODE));
    rb_define_const(mGimp, "FG_BG_HSV_MODE", INT2NUM(GIMP_FG_BG_HSV_MODE));
    rb_define_const(mGimp, "FG_TRANSPARENT_MODE", INT2NUM(GIMP_FG_TRANSPARENT_MODE));
    rb_define_const(mGimp, "CUSTOM_MODE", INT2NUM(GIMP_CUSTOM_MODE));

    //GimpBucketFillMode
    rb_define_const(mGimp, "FG_BUCKET_FILL", INT2NUM(GIMP_FG_BUCKET_FILL));
    rb_define_const(mGimp, "BG_BUCKET_FILL", INT2NUM(GIMP_BG_BUCKET_FILL));
    rb_define_const(mGimp, "PATTERN_BUCKET_FILL", INT2NUM(GIMP_PATTERN_BUCKET_FILL));

    //GimpChannelOps
    rb_define_const(mGimp, "CHANNEL_OP_ADD", INT2NUM(GIMP_CHANNEL_OP_ADD));
    rb_define_const(mGimp, "CHANNEL_OP_SUBTRACT", INT2NUM(GIMP_CHANNEL_OP_SUBTRACT));
    rb_define_const(mGimp, "CHANNEL_OP_REPLACE", INT2NUM(GIMP_CHANNEL_OP_REPLACE));
    rb_define_const(mGimp, "CHANNEL_OP_INTERSECT", INT2NUM(GIMP_CHANNEL_OP_INTERSECT));

    //GimpChannelType
    rb_define_const(mGimp, "RED_CHANNEL", INT2NUM(GIMP_RED_CHANNEL));
    rb_define_const(mGimp, "GREEN_CHANNEL", INT2NUM(GIMP_GREEN_CHANNEL));
    rb_define_const(mGimp, "BLUE_CHANNEL", INT2NUM(GIMP_BLUE_CHANNEL));
    rb_define_const(mGimp, "GRAY_CHANNEL", INT2NUM(GIMP_GRAY_CHANNEL));
    rb_define_const(mGimp, "INDEXED_CHANNEL", INT2NUM(GIMP_INDEXED_CHANNEL));
    rb_define_const(mGimp, "ALPHA_CHANNEL", INT2NUM(GIMP_ALPHA_CHANNEL));

    //GimpCheckSize
    rb_define_const(mGimp, "CHECK_SIZE_SMALL_CHECKS", INT2NUM(GIMP_CHECK_SIZE_SMALL_CHECKS));
    rb_define_const(mGimp, "CHECK_SIZE_MEDIUM_CHECKS", INT2NUM(GIMP_CHECK_SIZE_MEDIUM_CHECKS));
    rb_define_const(mGimp, "CHECK_SIZE_LARGE_CHECKS", INT2NUM(GIMP_CHECK_SIZE_LARGE_CHECKS));

    //GimpCheckType
    rb_define_const(mGimp, "CHECK_TYPE_LIGHT_CHECKS", INT2NUM(GIMP_CHECK_TYPE_LIGHT_CHECKS));
    rb_define_const(mGimp, "CHECK_TYPE_GRAY_CHECKS", INT2NUM(GIMP_CHECK_TYPE_GRAY_CHECKS));
    rb_define_const(mGimp, "CHECK_TYPE_DARK_CHECKS", INT2NUM(GIMP_CHECK_TYPE_DARK_CHECKS));
    rb_define_const(mGimp, "CHECK_TYPE_WHITE_ONLY", INT2NUM(GIMP_CHECK_TYPE_WHITE_ONLY));
    rb_define_const(mGimp, "CHECK_TYPE_GRAY_ONLY", INT2NUM(GIMP_CHECK_TYPE_GRAY_ONLY));
    rb_define_const(mGimp, "CHECK_TYPE_BLACK_ONLY", INT2NUM(GIMP_CHECK_TYPE_BLACK_ONLY));

    //GimpCloneType
    rb_define_const(mGimp, "IMAGE_CLONE", INT2NUM(GIMP_IMAGE_CLONE));
    rb_define_const(mGimp, "PATTERN_CLONE", INT2NUM(GIMP_PATTERN_CLONE));

    //GimpDesaturateMode
    rb_define_const(mGimp, "DESATURATE_LIGHTNESS", INT2NUM(GIMP_DESATURATE_LIGHTNESS));
    rb_define_const(mGimp, "DESATURATE_LUMINOSITY", INT2NUM(GIMP_DESATURATE_LUMINOSITY));
    rb_define_const(mGimp, "DESATURATE_AVERAGE", INT2NUM(GIMP_DESATURATE_AVERAGE));

    //GimpDodgeBurnType
    rb_define_const(mGimp, "DODGE", INT2NUM(GIMP_DODGE));
    rb_define_const(mGimp, "BURN", INT2NUM(GIMP_BURN));

    //GimpForegroundExtractMode
    rb_define_const(mGimp, "FOREGROUND_EXTRACT_SIOX", INT2NUM(GIMP_FOREGROUND_EXTRACT_SIOX));

    //GimpGradientType
    rb_define_const(mGimp, "GRADIENT_LINEAR", INT2NUM(GIMP_GRADIENT_LINEAR));
    rb_define_const(mGimp, "GRADIENT_BILINEAR", INT2NUM(GIMP_GRADIENT_BILINEAR));
    rb_define_const(mGimp, "GRADIENT_RADIAL", INT2NUM(GIMP_GRADIENT_RADIAL));
    rb_define_const(mGimp, "GRADIENT_SQUARE", INT2NUM(GIMP_GRADIENT_SQUARE));
    rb_define_const(mGimp, "GRADIENT_CONICAL_SYMMETRIC", INT2NUM(GIMP_GRADIENT_CONICAL_SYMMETRIC));
    rb_define_const(mGimp, "GRADIENT_CONICAL_ASYMMETRIC", INT2NUM(GIMP_GRADIENT_CONICAL_ASYMMETRIC));
    rb_define_const(mGimp, "GRADIENT_SHAPEBURST_ANGULAR", INT2NUM(GIMP_GRADIENT_SHAPEBURST_ANGULAR));
    rb_define_const(mGimp, "GRADIENT_SHAPEBURST_SPHERICAL", INT2NUM(GIMP_GRADIENT_SHAPEBURST_SPHERICAL));
    rb_define_const(mGimp, "GRADIENT_SHAPEBURST_DIMPLED", INT2NUM(GIMP_GRADIENT_SHAPEBURST_DIMPLED));
    rb_define_const(mGimp, "GRADIENT_SPIRAL_CLOCKWISE", INT2NUM(GIMP_GRADIENT_SPIRAL_CLOCKWISE));
    rb_define_const(mGimp, "GRADIENT_SPIRAL_ANTICLOCKWISE", INT2NUM(GIMP_GRADIENT_SPIRAL_ANTICLOCKWISE));

    //GimpGridStyle
    rb_define_const(mGimp, "GRID_DOTS", INT2NUM(GIMP_GRID_DOTS));
    rb_define_const(mGimp, "GRID_INTERSECTIONS", INT2NUM(GIMP_GRID_INTERSECTIONS));
    rb_define_const(mGimp, "GRID_ON_OFF_DASH", INT2NUM(GIMP_GRID_ON_OFF_DASH));
    rb_define_const(mGimp, "GRID_DOUBLE_DASH", INT2NUM(GIMP_GRID_DOUBLE_DASH));
    rb_define_const(mGimp, "GRID_SOLID", INT2NUM(GIMP_GRID_SOLID));

    //GimpIconType
    rb_define_const(mGimp, "ICON_TYPE_STOCK_ID", INT2NUM(GIMP_ICON_TYPE_STOCK_ID));
    rb_define_const(mGimp, "ICON_TYPE_INLINE_PIXBUF", INT2NUM(GIMP_ICON_TYPE_INLINE_PIXBUF));
    rb_define_const(mGimp, "ICON_TYPE_IMAGE_FILE", INT2NUM(GIMP_ICON_TYPE_IMAGE_FILE));

    //GimpImageBaseType
    rb_define_const(mGimp, "RGB", INT2NUM(GIMP_RGB));
    rb_define_const(mGimp, "GRAY", INT2NUM(GIMP_GRAY));
    rb_define_const(mGimp, "INDEXED", INT2NUM(GIMP_INDEXED));

    //GimpImageType
    rb_define_const(mGimp, "RGB_IMAGE", INT2NUM(GIMP_RGB_IMAGE));
    rb_define_const(mGimp, "RGBA_IMAGE", INT2NUM(GIMP_RGBA_IMAGE));
    rb_define_const(mGimp, "GRAY_IMAGE", INT2NUM(GIMP_GRAY_IMAGE));
    rb_define_const(mGimp, "GRAYA_IMAGE", INT2NUM(GIMP_GRAYA_IMAGE));
    rb_define_const(mGimp, "INDEXED_IMAGE", INT2NUM(GIMP_INDEXED_IMAGE));
    rb_define_const(mGimp, "INDEXEDA_IMAGE", INT2NUM(GIMP_INDEXEDA_IMAGE));

    //GimpInterpolationType
    rb_define_const(mGimp, "INTERPOLATION_NONE", INT2NUM(GIMP_INTERPOLATION_NONE));
    rb_define_const(mGimp, "INTERPOLATION_LINEAR", INT2NUM(GIMP_INTERPOLATION_LINEAR));
    rb_define_const(mGimp, "INTERPOLATION_CUBIC", INT2NUM(GIMP_INTERPOLATION_CUBIC));
    rb_define_const(mGimp, "INTERPOLATION_LANCZOS", INT2NUM(GIMP_INTERPOLATION_LANCZOS));

    //GimpMessageHandlerType
    rb_define_const(mGimp, "MESSAGE_BOX", INT2NUM(GIMP_MESSAGE_BOX));
    rb_define_const(mGimp, "CONSOLE", INT2NUM(GIMP_CONSOLE));
    rb_define_const(mGimp, "ERROR_CONSOLE", INT2NUM(GIMP_ERROR_CONSOLE));

    //GimpPaintApplicationMode
    rb_define_const(mGimp, "PAINT_CONSTANT", INT2NUM(GIMP_PAINT_CONSTANT));
    rb_define_const(mGimp, "PAINT_INCREMENTAL", INT2NUM(GIMP_PAINT_INCREMENTAL));

    //GimpPDBArgType
    rb_define_const(mGimp, "PDB_INT32", INT2NUM(GIMP_PDB_INT32));
    rb_define_const(mGimp, "PDB_INT16", INT2NUM(GIMP_PDB_INT16));
    rb_define_const(mGimp, "PDB_INT8", INT2NUM(GIMP_PDB_INT8));
    rb_define_const(mGimp, "PDB_FLOAT", INT2NUM(GIMP_PDB_FLOAT));
    rb_define_const(mGimp, "PDB_STRING", INT2NUM(GIMP_PDB_STRING));
    rb_define_const(mGimp, "PDB_INT32ARRAY", INT2NUM(GIMP_PDB_INT32ARRAY));
    rb_define_const(mGimp, "PDB_INT16ARRAY", INT2NUM(GIMP_PDB_INT16ARRAY));
    rb_define_const(mGimp, "PDB_INT8ARRAY", INT2NUM(GIMP_PDB_INT8ARRAY));
    rb_define_const(mGimp, "PDB_FLOATARRAY", INT2NUM(GIMP_PDB_FLOATARRAY));
    rb_define_const(mGimp, "PDB_STRINGARRAY", INT2NUM(GIMP_PDB_STRINGARRAY));
    rb_define_const(mGimp, "PDB_COLOR", INT2NUM(GIMP_PDB_COLOR));
    rb_define_const(mGimp, "PDB_REGION", INT2NUM(GIMP_PDB_REGION));
    rb_define_const(mGimp, "PDB_DISPLAY", INT2NUM(GIMP_PDB_DISPLAY));
    rb_define_const(mGimp, "PDB_IMAGE", INT2NUM(GIMP_PDB_IMAGE));
    rb_define_const(mGimp, "PDB_LAYER", INT2NUM(GIMP_PDB_LAYER));
    rb_define_const(mGimp, "PDB_CHANNEL", INT2NUM(GIMP_PDB_CHANNEL));
    rb_define_const(mGimp, "PDB_DRAWABLE", INT2NUM(GIMP_PDB_DRAWABLE));
    rb_define_const(mGimp, "PDB_SELECTION", INT2NUM(GIMP_PDB_SELECTION));
    rb_define_const(mGimp, "PDB_BOUNDARY", INT2NUM(GIMP_PDB_BOUNDARY));
    rb_define_const(mGimp, "PDB_VECTORS", INT2NUM(GIMP_PDB_VECTORS));
    rb_define_const(mGimp, "PDB_PARASITE", INT2NUM(GIMP_PDB_PARASITE));
    rb_define_const(mGimp, "PDB_STATUS", INT2NUM(GIMP_PDB_STATUS));
    rb_define_const(mGimp, "PDB_END", INT2NUM(GIMP_PDB_END));

    //GimpPDBProcType
    rb_define_const(mGimp, "INTERNAL", INT2NUM(GIMP_INTERNAL));
    rb_define_const(mGimp, "PLUGIN", INT2NUM(GIMP_PLUGIN));
    rb_define_const(mGimp, "EXTENSION", INT2NUM(GIMP_EXTENSION));
    rb_define_const(mGimp, "TEMPORARY", INT2NUM(GIMP_TEMPORARY));

    //GimpPDBStatusType
    rb_define_const(mGimp, "PDB_EXECUTION_ERROR", INT2NUM(GIMP_PDB_EXECUTION_ERROR));
    rb_define_const(mGimp, "PDB_CALLING_ERROR", INT2NUM(GIMP_PDB_CALLING_ERROR));
    rb_define_const(mGimp, "PDB_PASS_THROUGH", INT2NUM(GIMP_PDB_PASS_THROUGH));
    rb_define_const(mGimp, "PDB_SUCCESS", INT2NUM(GIMP_PDB_SUCCESS));
    rb_define_const(mGimp, "PDB_CANCEL", INT2NUM(GIMP_PDB_CANCEL));

    //GimpProgressCommand
    rb_define_const(mGimp, "PROGRESS_COMMAND_START", INT2NUM(GIMP_PROGRESS_COMMAND_START));
    rb_define_const(mGimp, "PROGRESS_COMMAND_END", INT2NUM(GIMP_PROGRESS_COMMAND_END));
    rb_define_const(mGimp, "PROGRESS_COMMAND_SET_TEXT", INT2NUM(GIMP_PROGRESS_COMMAND_SET_TEXT));
    rb_define_const(mGimp, "PROGRESS_COMMAND_SET_VALUE", INT2NUM(GIMP_PROGRESS_COMMAND_SET_VALUE));
    rb_define_const(mGimp, "PROGRESS_COMMAND_PULSE", INT2NUM(GIMP_PROGRESS_COMMAND_PULSE));
    rb_define_const(mGimp, "PROGRESS_COMMAND_GET_WINDOW", INT2NUM(GIMP_PROGRESS_COMMAND_GET_WINDOW));

    //GimpRepeatMode
    rb_define_const(mGimp, "REPEAT_NONE", INT2NUM(GIMP_REPEAT_NONE));
    rb_define_const(mGimp, "REPEAT_SAWTOOTH", INT2NUM(GIMP_REPEAT_SAWTOOTH));
    rb_define_const(mGimp, "REPEAT_TRIANGULAR", INT2NUM(GIMP_REPEAT_TRIANGULAR));

    //GimpRunMode
    rb_define_const(mGimp, "RUN_INTERACTIVE", INT2NUM(GIMP_RUN_INTERACTIVE));
    rb_define_const(mGimp, "RUN_NONINTERACTIVE", INT2NUM(GIMP_RUN_NONINTERACTIVE));
    rb_define_const(mGimp, "RUN_WITH_LAST_VALS", INT2NUM(GIMP_RUN_WITH_LAST_VALS));

    //GimpSizeType
    rb_define_const(mGimp, "PIXELS", INT2NUM(GIMP_PIXELS));
    rb_define_const(mGimp, "POINTS", INT2NUM(GIMP_POINTS));

    //GimpStateTraceMode
    rb_define_const(mGimp, "STACK_TRACE_NEVER", INT2NUM(GIMP_STACK_TRACE_NEVER));
    rb_define_const(mGimp, "STACK_TRACE_QUERY", INT2NUM(GIMP_STACK_TRACE_QUERY));
    rb_define_const(mGimp, "STACK_TRACE_ALWAYS", INT2NUM(GIMP_STACK_TRACE_ALWAYS));

    //GimpTransferMode
    rb_define_const(mGimp, "SHADOWS", INT2NUM(GIMP_SHADOWS));
    rb_define_const(mGimp, "MIDTONES", INT2NUM(GIMP_MIDTONES));
    rb_define_const(mGimp, "HIGHLIGHTS", INT2NUM(GIMP_HIGHLIGHTS));

    //GimpTransformDirection
    rb_define_const(mGimp, "TRANSFORM_FORWARD", INT2NUM(GIMP_TRANSFORM_FORWARD));
    rb_define_const(mGimp, "TRANSFORM_BACKWARD", INT2NUM(GIMP_TRANSFORM_BACKWARD));

  //GimpBrushApplicationMode
    rb_define_const(mGimp, "BRUSH_HARD", INT2NUM(GIMP_BRUSH_HARD));
    rb_define_const(mGimp, "BRUSH_SOFT", INT2NUM(GIMP_BRUSH_SOFT));

  //GimpBrushGeneratedShape
    rb_define_const(mGimp, "BRUSH_GENERATED_CIRCLE", INT2NUM(GIMP_BRUSH_GENERATED_CIRCLE));
    rb_define_const(mGimp, "BRUSH_GENERATED_SQUARE", INT2NUM(GIMP_BRUSH_GENERATED_SQUARE));
    rb_define_const(mGimp, "BRUSH_GENERATED_DIAMOND", INT2NUM(GIMP_BRUSH_GENERATED_DIAMOND));

  //GimpConvertDitherType
    rb_define_const(mGimp, "NO_DITHER", INT2NUM(GIMP_NO_DITHER));
    rb_define_const(mGimp, "FS_DITHER", INT2NUM(GIMP_FS_DITHER));
    rb_define_const(mGimp, "FSLOWBLEED_DITHER", INT2NUM(GIMP_FSLOWBLEED_DITHER));
    rb_define_const(mGimp, "FIXED_DITHER", INT2NUM(GIMP_FIXED_DITHER));

  //GimpConvertPaletteType
    rb_define_const(mGimp, "MAKE_PALETTE", INT2NUM(GIMP_MAKE_PALETTE));
    rb_define_const(mGimp, "REUSE_PALETTE", INT2NUM(GIMP_REUSE_PALETTE));
    rb_define_const(mGimp, "WEB_PALETTE", INT2NUM(GIMP_WEB_PALETTE));
    rb_define_const(mGimp, "MONO_PALETTE", INT2NUM(GIMP_MONO_PALETTE));
    rb_define_const(mGimp, "CUSTOM_PALETTE", INT2NUM(GIMP_CUSTOM_PALETTE));

  //GimpConvolutionType
    rb_define_const(mGimp, "NORMAL_CONVOL", INT2NUM(GIMP_NORMAL_CONVOL));
    rb_define_const(mGimp, "ABSOLUTE_CONVOL", INT2NUM(GIMP_ABSOLUTE_CONVOL));
    rb_define_const(mGimp, "NEGATIVE_CONVOL", INT2NUM(GIMP_NEGATIVE_CONVOL));

  //GimpConvolveType
    rb_define_const(mGimp, "BLUR_CONVOLVE", INT2NUM(GIMP_BLUR_CONVOLVE));
    rb_define_const(mGimp, "SHARPEN_CONVOLVE", INT2NUM(GIMP_SHARPEN_CONVOLVE));

  //GimpFillType
    rb_define_const(mGimp, "FOREGROUND_FILL", INT2NUM(GIMP_FOREGROUND_FILL));
    rb_define_const(mGimp, "BACKGROUND_FILL", INT2NUM(GIMP_BACKGROUND_FILL));
    rb_define_const(mGimp, "WHITE_FILL", INT2NUM(GIMP_WHITE_FILL));
    rb_define_const(mGimp, "TRANSPARENT_FILL", INT2NUM(GIMP_TRANSPARENT_FILL));
    rb_define_const(mGimp, "PATTERN_FILL", INT2NUM(GIMP_PATTERN_FILL));

  //GimpGradientSegmentColor
    rb_define_const(mGimp, "GRADIENT_SEGMENT_RGB", INT2NUM(GIMP_GRADIENT_SEGMENT_RGB));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_HSV_CCW", INT2NUM(GIMP_GRADIENT_SEGMENT_HSV_CCW));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_HSV_CW", INT2NUM(GIMP_GRADIENT_SEGMENT_HSV_CW));

  //GimpGradientSegmentType
    rb_define_const(mGimp, "GRADIENT_SEGMENT_LINEAR", INT2NUM(GIMP_GRADIENT_SEGMENT_LINEAR));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_CURVED", INT2NUM(GIMP_GRADIENT_SEGMENT_CURVED));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_SINE", INT2NUM(GIMP_GRADIENT_SEGMENT_SINE));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_SPHERE_INCREASING", INT2NUM(GIMP_GRADIENT_SEGMENT_SPHERE_INCREASING));
    rb_define_const(mGimp, "GRADIENT_SEGMENT_SPHERE_DECREASING", INT2NUM(GIMP_GRADIENT_SEGMENT_SPHERE_DECREASING));

  //GimpHistogramChannel
    rb_define_const(mGimp, "HISTOGRAM_VALUE", INT2NUM(GIMP_HISTOGRAM_VALUE));
    rb_define_const(mGimp, "HISTOGRAM_RED", INT2NUM(GIMP_HISTOGRAM_RED));
    rb_define_const(mGimp, "HISTOGRAM_GREEN", INT2NUM(GIMP_HISTOGRAM_GREEN));
    rb_define_const(mGimp, "HISTOGRAM_BLUE", INT2NUM(GIMP_HISTOGRAM_BLUE));
    rb_define_const(mGimp, "HISTOGRAM_ALPHA", INT2NUM(GIMP_HISTOGRAM_ALPHA));

  //GimpHueRange
    rb_define_const(mGimp, "ALL_HUES", INT2NUM(GIMP_ALL_HUES));
    rb_define_const(mGimp, "RED_HUES", INT2NUM(GIMP_RED_HUES));
    rb_define_const(mGimp, "YELLOW_HUES", INT2NUM(GIMP_YELLOW_HUES));
    rb_define_const(mGimp, "GREEN_HUES", INT2NUM(GIMP_GREEN_HUES));
    rb_define_const(mGimp, "CYAN_HUES", INT2NUM(GIMP_CYAN_HUES));
    rb_define_const(mGimp, "BLUE_HUES", INT2NUM(GIMP_BLUE_HUES));
    rb_define_const(mGimp, "MAGENTA_HUES", INT2NUM(GIMP_MAGENTA_HUES));

  //GimpLayerModeEffects
    rb_define_const(mGimp, "NORMAL_MODE", INT2NUM(GIMP_NORMAL_MODE));
    rb_define_const(mGimp, "DISSOLVE_MODE", INT2NUM(GIMP_DISSOLVE_MODE));
    rb_define_const(mGimp, "BEHIND_MODE", INT2NUM(GIMP_BEHIND_MODE));
    rb_define_const(mGimp, "MULTIPLY_MODE", INT2NUM(GIMP_MULTIPLY_MODE));
    rb_define_const(mGimp, "SCREEN_MODE", INT2NUM(GIMP_SCREEN_MODE));
    rb_define_const(mGimp, "OVERLAY_MODE", INT2NUM(GIMP_OVERLAY_MODE));
    rb_define_const(mGimp, "DIFFERENCE_MODE", INT2NUM(GIMP_DIFFERENCE_MODE));
    rb_define_const(mGimp, "ADDITION_MODE", INT2NUM(GIMP_ADDITION_MODE));
    rb_define_const(mGimp, "SUBTRACT_MODE", INT2NUM(GIMP_SUBTRACT_MODE));
    rb_define_const(mGimp, "DARKEN_ONLY_MODE", INT2NUM(GIMP_DARKEN_ONLY_MODE));
    rb_define_const(mGimp, "LIGHTEN_ONLY_MODE", INT2NUM(GIMP_LIGHTEN_ONLY_MODE));
    rb_define_const(mGimp, "HUE_MODE", INT2NUM(GIMP_HUE_MODE));
    rb_define_const(mGimp, "SATURATION_MODE", INT2NUM(GIMP_SATURATION_MODE));
    rb_define_const(mGimp, "COLOR_MODE", INT2NUM(GIMP_COLOR_MODE));
    rb_define_const(mGimp, "VALUE_MODE", INT2NUM(GIMP_VALUE_MODE));
    rb_define_const(mGimp, "DIVIDE_MODE", INT2NUM(GIMP_DIVIDE_MODE));
    rb_define_const(mGimp, "DODGE_MODE", INT2NUM(GIMP_DODGE_MODE));
    rb_define_const(mGimp, "BURN_MODE", INT2NUM(GIMP_BURN_MODE));
    rb_define_const(mGimp, "HARDLIGHT_MODE", INT2NUM(GIMP_HARDLIGHT_MODE));
    rb_define_const(mGimp, "SOFTLIGHT_MODE", INT2NUM(GIMP_SOFTLIGHT_MODE));
    rb_define_const(mGimp, "GRAIN_EXTRACT_MODE", INT2NUM(GIMP_GRAIN_EXTRACT_MODE));
    rb_define_const(mGimp, "GRAIN_MERGE_MODE", INT2NUM(GIMP_GRAIN_MERGE_MODE));
    rb_define_const(mGimp, "COLOR_ERASE_MODE", INT2NUM(GIMP_COLOR_ERASE_MODE));

  //GimpMaskApplyMode
    rb_define_const(mGimp, "MASK_APPLY", INT2NUM(GIMP_MASK_APPLY));
    rb_define_const(mGimp, "MASK_DISCARD", INT2NUM(GIMP_MASK_DISCARD));

  //GimpMergeType
    rb_define_const(mGimp, "EXPAND_AS_NECESSARY", INT2NUM(GIMP_EXPAND_AS_NECESSARY));
    rb_define_const(mGimp, "CLIP_TO_IMAGE", INT2NUM(GIMP_CLIP_TO_IMAGE));
    rb_define_const(mGimp, "CLIP_TO_BOTTOM_LAYER", INT2NUM(GIMP_CLIP_TO_BOTTOM_LAYER));
    rb_define_const(mGimp, "FLATTEN_IMAGE", INT2NUM(GIMP_FLATTEN_IMAGE));

  //GimpOffsetType
    rb_define_const(mGimp, "OFFSET_BACKGROUND", INT2NUM(GIMP_OFFSET_BACKGROUND));
    rb_define_const(mGimp, "OFFSET_TRANSPARENT", INT2NUM(GIMP_OFFSET_TRANSPARENT));

  //GimpOrientationType
    rb_define_const(mGimp, "ORIENTATION_HORIZONTAL", INT2NUM(GIMP_ORIENTATION_HORIZONTAL));
    rb_define_const(mGimp, "ORIENTATION_VERTICAL", INT2NUM(GIMP_ORIENTATION_VERTICAL));
    rb_define_const(mGimp, "ORIENTATION_UNKNOWN", INT2NUM(GIMP_ORIENTATION_UNKNOWN));

  //GimpRotationType
    rb_define_const(mGimp, "ROTATE_90", INT2NUM(GIMP_ROTATE_90));
    rb_define_const(mGimp, "ROTATE_180", INT2NUM(GIMP_ROTATE_180));
    rb_define_const(mGimp, "ROTATE_270", INT2NUM(GIMP_ROTATE_270));
}
