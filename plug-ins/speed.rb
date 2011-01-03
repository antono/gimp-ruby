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

tim = Time.now

require "rubyfu"

RubyFu.register(:name => "ruby-fu-speed", :menupath => "Speed") do
  Gimp.message "Plugin startup time is #{Time.now - tim} seconds."
  nil
end

RubyFu.menu_register("ruby-fu-speed", "<Toolbox>/Xtns/Languages/Ruby-Fu/Test")
