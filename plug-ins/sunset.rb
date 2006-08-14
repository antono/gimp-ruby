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

require 'rubyfu'

include Gimp
include RubyFu

register(
  'ruby-fu-sunset', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  'Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Sunset', #menupath
  nil, #image types
  [
    ParamDef.INT32('width', 'Width', 640),
    ParamDef.INT32('height', 'Height', 480),
  ], #params
  [
    ParamDef.IMAGE('image', 'Image'),
  ] #results
) do|run_mode, w, h|
  include PDB::Access

  image = Image.new(w, h, RGB)
  
  Context.push do
    sunset = Layer.new(image, w, h, RGB_IMAGE, 'sunset', 100, NORMAL_MODE)
    image.add_layer(sunset, nil)
      
    fg = Color(0.025, 0.000, 0.219)
    bg = Color(1.000, 0.870, 0.000)
    
    Context.set_foreground(fg)
    Context.set_background(bg)
    Context.set_gradient('FG to BG (HSV counter-clockwise)')
  
    gimp_rect_select(image, 0, 0, w, h/3, CHANNEL_OP_REPLACE, false, 0.0)
    Edit.blend(sunset, CUSTOM_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, 0, 0, h/3)
    gimp_rect_select(image, 0, h/3, w, h, CHANNEL_OP_REPLACE, false, 0.0)
    Edit.blend(sunset, CUSTOM_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, h, 0, h/3)
    Selection.none(image)
  
    waves = Layer.new(image, w, h, RGB_IMAGE, 'waves', 100, NORMAL_MODE)
    image.add_layer(waves, nil)
    plug_in_solid_noise(image, waves, false, false, rand(10_000), 15, 1.5, 16)    
    
    Context.set_foreground(Color(0.5, 0.5, 0.5))
    Edit.blend(waves, FG_TRANSPARENT_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, h/3, 0, h)
    plug_in_displace(image, sunset, 0, h/4, false, true, nil, waves, 1)
    
    plug_in_bump_map(image, sunset, waves, 270, 50, 6, 0, 0, 0, 111, true, false, 0)
    image.remove_layer(waves)
  
    gimp_rect_select(image, 0, h/3, w, h, CHANNEL_OP_REPLACE, false, 0.0)
    gimp_levels(sunset, HISTOGRAM_VALUE, 0, 255, 0.65, 0, 255)
    Selection.none(image)
  
    Display.new(image)
  end

  [image]
end
