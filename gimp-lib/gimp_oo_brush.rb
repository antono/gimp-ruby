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

require 'gimp_oo.rb'

module Gimp
  class_blacklist = [
    'get_brush',
    'get_brush_data',
    'get_opacity',
    'get_paint_mode',
    'get_spacing',
    'list',
    'set_brush',
    'set_opacity',
    'set_paint_mode',
    'set_spacing',
  ]
  
  Brush = GimpOO::ClassTemplate.template('gimp-brush-', class_blacklist,
                                         'gimp-brushes-', [])
end