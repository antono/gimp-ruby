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

manifest = {
  :name       => "ruby-fu-gloom",
  :blurb      => _("Gives an image a darkened and softened cartoon look"),
  :help       => _("Takes a radius and intensity for the effect"),
  :author     => "Scott Lembcke",
  :copyright  => "Scott Lembcke",
  :date       => "2006",
  :menupath   => _("Gloom"),
  :imagetypes => "*",
  :params => [
    RubyFu::ParamDef.SLIDER("radius", _("Radius"), 5.0, 0..50, 0.1),
    RubyFu::ParamDef.SLIDER("amount", _("Amount"), 100, 0..100, 1),
  ]
}

RubyFu.register(manifest) do |run_mode, image, drawable, radius, amount|
  include PDB::Access
  
  image.undo_group_start do
    gloom = Layer.new_from_drawable(drawable, image)
    image.add_layer(gloom, nil)
    gimp_drawable_set_name(gloom, _("gloom"))
    gloom.set_mode(DARKEN_ONLY_MODE)
    
    plug_in_gauss_iir2(image, gloom, radius, radius)
  end
  
  Gimp::Display.flush
end

RubyFu.menu_register("ruby-fu-gloom", "<Image>/Ruby-Fu/")
