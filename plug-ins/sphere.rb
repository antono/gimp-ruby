#!/usr/bin/env ruby

# GIMP-Ruby -- Allows GIMP plugins to be written in Ruby.
# Copyright (C) 2006  Scott Lembcke
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor,Boston, MA
# 02110-1301, USA.

require "rubyfu"
include Gimp
include RubyFu

include PDB::Access

# converted from sphere.scm
# Everyone else on the block had a sphere generator.
# I didn't want Gimp-Ruby to feel left out.

RubyFu.register(
  :name       => "ruby-fu-sphere",
  :blurb      => _("Simple sphere with a drop shadow"),
  :help       => _("Simple sphere with a drop shadow"),
  :author     => "Spencer Kimball and Scott Lembcke",
  :copyright  => "Spencer Kimball",
  :date       => "1996",
  :menupath   => _("Sphere"),
  :imagetypes => "",
  :params     => [
    ParamDef.SPINNER("radius", _("Radius (pixels)"), 100, 0..1000, 1),
    ParamDef.SLIDER("lighting", _("Lighting (degrees)"), 45, 0..360, 1),
    ParamDef.TOGGLE("shadow", _("Shadow"), 1),
    ParamDef.COLOR("bg color", _("Background Color"), Color(1.0, 1.0, 1.0)),
    ParamDef.COLOR("color", _("Sphere Color"), Color(1.0, 0.0, 0.0)),
  ],
  :results => [ParamDef.IMAGE("image", _("Sphere Image"))]
) do |run_mode, radius, light, shadow, bg_color, sphere_color|
  shadow = (shadow == 1)
  
  width  = radius * 3.75
  height = radius * 2.5

  img = Image.new(width, height, 0)

  Context.push do
    img.undo_disable do
      radians = light * Math::PI / 180
      cx = width / 2
      cy = height / 2
      light_x = cx + radius * 0.6 * Math.cos(radians)
      light_y = cy - radius * 0.6 * Math.sin(radians)
      light_end_x = cx + radius * Math.cos(radians + Math::PI)
      light_end_y = cy - radius * Math.sin(radians + Math::PI)
      offset = radius * 0.1
  
      layer = Layer.new(img, width, height, RGB_IMAGE,
                        _("Sphere Layer"), 100, NORMAL_MODE)
      img.add_layer(layer, 0)
      
      Context.set_foreground(sphere_color)
      Context.set_background(bg_color)
      Edit.fill(layer, BACKGROUND_FILL)
  
      Context.set_background(Color(0.1, 0.1, 0.1))
  
      if shadow and ((45 <= light and light <= 75) or
                    (105 <= light and light <= 135))
        shadow_w = radius * 2.5 * Math.cos(radians + Math::PI)
        shadow_h = radius * 0.5
        shadow_x = cx
        shadow_y = radius * 0.65 + cy
        if shadow_w < 0
          shadow_x = shadow_w + cx
          shadow_w = - shadow_w
        end
        
        gimp_ellipse_select(img, shadow_x, shadow_y, shadow_w, shadow_h,
                            CHANNEL_OP_REPLACE, true, true, 7.5)
        Edit.bucket_fill(layer, BG_BUCKET_FILL, MULTIPLY_MODE, 100, 0, false, 0, 0)
      end
  
      PDB.gimp_ellipse_select(img, cx - radius, cy - radius, radius * 2, radius * 2,
                              CHANNEL_OP_REPLACE, true, false, 0)
      Edit.blend(layer, FG_BG_RGB_MODE, NORMAL_MODE, GRADIENT_RADIAL, 100, offset,
                 REPEAT_NONE, false, false, 0, 0, true, 
                 light_x, light_y, light_end_x, light_end_y)
      Selection.none(img)
    end
  end
  
  Display.new(img)
  
  img
end

RubyFu.menu_register("ruby-fu-sphere", ExamplesMenu)
