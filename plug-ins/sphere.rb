#!/usr/bin/env ruby

=begin
Translated into Ruby by Masahiro Sakai <s01397ms@sfc.keio.acjp>, 26th Feb 2001
=end

=begin header
Create a 3D sphere with optional shadow
The sphere's principle color will be the foreground
Parameters:
 bg-color: background color
 sphere-color: color of sphere
 radius: radius of the sphere in pixels
 light:  angle of light source in degrees
 shadow: whather to create a shadow as well'
=end

require "gimp"
include Gimp

#include PDB::Access

include Math

def run(run_mode, radius = 100, light = 45, shadow = true, bg_color = Gimp::Rgb.new(1.0, 1.0, 1.0), sphere_color = Gimp::Rgb.new(1.0, 0.0, 0.0))
  width  = radius * 3.75
  height = radius * 2.5

  img = PDB.gimp_image_new(width, height, 0)

  PDB.gimp_context_push()
  PDB.gimp_image_undo_disable(img)

  old_fg = PDB.gimp_context_get_foreground
  old_bg = PDB.gimp_context_get_background

  begin
    radians = light * PI / 180
    cx = width / 2
    cy = height / 2
    light_x = cx + radius * 0.6 * cos(radians)
    light_y = cy - radius * 0.6 * sin(radians)
    light_end_x = cx + radius * cos(radians + PI)
    light_end_y = cy - radius * sin(radians + PI)
    offset = radius * 0.1

    drawable = PDB.gimp_layer_new(img, width, height, RGB_IMAGE, "sphere Layer", 100, NORMAL_MODE)
    PDB.gimp_image_add_layer(img, drawable, 0)
    
    PDB.gimp_context_set_foreground(sphere_color)
    PDB.gimp_context_set_background(bg_color)
    PDB.gimp_edit_fill(drawable, BACKGROUND_FILL)

    PDB.gimp_context_set_background(Gimp::Rgb.new(0.1, 0.1, 0.1))

    if shadow and ((45 <= light and light <= 75) or
                   (105 <= light and light <= 135))
      shadow_w = radius * 2.5 * cos(radians + PI)
      shadow_h = radius * 0.5
      shadow_x = cx
      shadow_y = radius * 0.65 + cy
      if shadow_w < 0
        shadow_x = shadow_w + cx
        shadow_w = - shadow_w
      end
      PDB.gimp_ellipse_select(img, shadow_x, shadow_y, shadow_w, shadow_h,
			  0, 1, 1, 7.5)
      PDB.gimp_edit_bucket_fill(drawable, 1, 3, 100, 0, 0, 0, 0)
    end

    PDB.gimp_ellipse_select(img, cx - radius, cy - radius, radius * 2, radius * 2,
			2, 1, 0, 0)
    PDB.gimp_edit_blend(drawable, 0, 0, 2, 100, offset,
	       0, 0, 0, 0, 0, 0, light_x, light_y,
	       light_end_x, light_end_y)
    PDB.gimp_selection_none(img)
  ensure
    PDB.gimp_context_set_background(old_bg)
    PDB.gimp_context_set_foreground(old_fg)
    PDB.gimp_image_undo_enable(img)
  end

  PDB.gimp_context_pop()
  PDB.gimp_display_new(img)
  
  return img
end


RubyFu.register(
  "ruby_fu_sphere",
  "Simple sphere with a drop shadow",
  "Simple sphere with a drop shadow", 
  "Spencer Kimball and Masahiro Sakai",
  "Spencer Kimball",
  "2001",
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Sphere',
  "",
  [
    ParamDef.INT32('run-mode', 'Run mode'),
    ParamDef.FLOAT('radius', 'Radius'),
    ParamDef.FLOAT('lighting', 'Lighting Degrees'),
    ParamDef.INT32('shadow', 'Shadow'),
    ParamDef.COLOR('bg color', 'Background Color'),
    ParamDef.COLOR('color', 'Sphere Color'),
  ],
  [ParamDef.IMAGE('image', 'Result Image')],
  &method(:run)
)

RubyFu.main
