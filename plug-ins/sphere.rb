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
include RubyFu

#include PDB::Access

include Math

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
    ParamDef.SPINNER('radius', 'Radius', 100, 0..1000, 1),
    ParamDef.SLIDER('lighting', 'Lighting Degrees', 45, 0..360, 1),
    ParamDef.TOGGLE('shadow', 'Shadow', 1),
    ParamDef.COLOR('bg color', 'Background Color', Color(1.0, 1.0, 1.0)),
    ParamDef.COLOR('color', 'Sphere Color', Color(1.0, 0.0, 0.0)),
  ],
  [ParamDef.IMAGE('image', 'Result Image')]
) do |run_mode, radius, light, shadow, bg_color, sphere_color|
  require 'gimp_oo'
  
  shadow = (shadow == 1)
  
  width  = radius * 3.75
  height = radius * 2.5

  img = Image.new(width, height, 0)

  Context.push do
    img.undo_disable do
      radians = light * PI / 180
      cx = width / 2
      cy = height / 2
      light_x = cx + radius * 0.6 * cos(radians)
      light_y = cy - radius * 0.6 * sin(radians)
      light_end_x = cx + radius * cos(radians + PI)
      light_end_y = cy - radius * sin(radians + PI)
      offset = radius * 0.1
  
      drawable = Layer.new(img, width, height, RGB_IMAGE, "sphere Layer", 100, NORMAL_MODE)
      img.add_layer(drawable, 0)
      
      Context.set_foreground(sphere_color)
      Context.set_background(bg_color)
      Edit.fill(drawable, BACKGROUND_FILL)
  
      Context.set_background(Color(0.1, 0.1, 0.1))
  
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
        Edit.bucket_fill(drawable, 1, 3, 100, 0, 0, 0, 0)
      end
  
      PDB.gimp_ellipse_select(img, cx - radius, cy - radius, radius * 2, radius * 2,
        2, 1, 0, 0)
      Edit.blend(drawable, 0, 0, 2, 100, offset,
           0, 0, 0, 0, 0, 0, light_x, light_y,
           light_end_x, light_end_y)
      Selection.none(img)
    end
  end
  
  Display.new(img)
  
  img
end

main
