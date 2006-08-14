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

Size = 256

include Gimp
include RubyFu

register(
  'ruby-fu-gloom', #procedure name
  '', #blurb
  '', #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Image>/Ruby-Fu/Gloom', #menupath
  '*', #image types
  [
    ParamDef.SLIDER('radius', 'Radius', 5.0, 0..50, 0.1),
    ParamDef.SLIDER('amount', 'Amount', 100, 0..100, 1),
  ], #params
  [] #results
) do|run_mode, image, drawable, radius, amount|
  include PDB::Access
  
  image.undo_group do
    gloom = Layer.new_from_drawable(drawable, image)
    image.add_layer(gloom, nil)
    gimp_drawable_set_name(gloom, 'gloom')
    gloom.set_mode(DARKEN_ONLY_MODE)
    
    plug_in_gauss_iir2(image, gloom, radius, radius)
  end
  
  Display.flush
  []
end
