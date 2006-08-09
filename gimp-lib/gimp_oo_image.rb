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
  Image = GimpOO::ClassTemplate.template('gimp-image-', nil)
  
  class Image
    add_class_method('list', 'gimp-image-list')
  
    alias_method :old_undo_group_start, :undo_group_start
    def undo_group
      old_undo_group_start
      if block_given?
        begin
          yield
        ensure
          undo_group_end
        end #begin
      end #if
    end #def
    
    alias_method :old_undo_disable, :undo_disable
    def undo_disable
      old_undo_disable
      if block_given?
        begin
          yield
        ensure
          undo_enable
        end
      end #if
    end #def
  end #class
end #module