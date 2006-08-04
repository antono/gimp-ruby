/* GIMP-Ruby -- Allows GIMP plugins to be written in Ruby.
 * Copyright (C) 2006  Scott Lembcke
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,Boston, MA
 * 02110-1301, USA.
 */
 
/* modules */
extern VALUE mGimp;


/* classes */
extern VALUE cGimpRGB;
extern VALUE cGimpHSV;
extern VALUE cGimpCMYK;


/* structures (structures.c) */
extern VALUE sGimpParamDef;
extern VALUE sGimpParam;
extern VALUE sGimpPlugInInfo;
extern VALUE sGimpParasite;


/* Type conversion functions (conversion.c) */
VALUE       GimpParam2rb   (const GimpParam *param,
                            int              index);
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

VALUE    GimpRGB2rb     (const GimpRGB *color);
GimpRGB *rb2GimpRGBPtr  (VALUE          color);
GimpRGB  rb2GimpRGB     (VALUE          color);

VALUE    GimpHSV2rb     (const GimpHSV *color);
GimpHSV *rb2GimpHSVPtr  (VALUE          color);
GimpHSV  rb2GimpHSV     (VALUE          color);

VALUE     GimpCMYK2rb     (const GimpCMYK *color);
GimpCMYK *rb2GimpCMYKPtr  (VALUE           color);
GimpCMYK  rb2GimpCMYK     (VALUE           color);

VALUE        GimpParasite2rb (GimpParasite leech);
GimpParasite rb2GimpParasite (VALUE        leech);


/* ids (ids.c) */
extern ID id_type;
extern ID id_name;
extern ID id_dscr;
extern ID id_data;
extern ID id_init_proc;
extern ID id_quit_proc;
extern ID id_query_proc;
extern ID id_run_proc;
extern ID id_call;
extern ID id_flags;


/* Init functions */
void Init_structures(void);
void Init_ids(void);

void Init_gimpconstants(void);
void Init_gimpmain(void);
void Init_gimpproceduraldb(void);

void Init_gimpcolorrgb(void);
void Init_gimpcolorhsv(void);
void Init_gimpcolorcmyk(void);
void Init_gimpcolorspace(void);

void Init_rubyfudialog(void);
