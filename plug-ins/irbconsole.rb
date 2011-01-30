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

RubyFu.register(
  :name      => "ruby-fu-console",
  :blurb     => _("Starts an irb session in a console."),
  :help      => _("Starts an irb session in a console."),
  :author    => "Scott Lembcke",
  :copyright => "Scott Lembcke",
  :date      => "2006",
  :menupath  => _("Ruby Console")
) do

  require "irb"
  
  include Gimp
  include PDB::Access

  console = File.join(GIMP_PLUG_IN_DIRECTORY, "ruby", "ruby-fu-console")
  $stdin = $stdout = open("|#{console}", "w+")

  puts _("Interactive Gimp-Ruby Console")
  puts _("Ruby version #{RUBY_VERSION}")
  puts

  ARGV.clear

  IRB.setup(nil)
  irb = IRB::Irb.new(nil, IRB::StdioInputMethod.new)
  IRB.conf[:MAIN_CONTEXT] = irb.context
  
  IRB.conf[:AUTO_INDENT] = true
  IRB.conf[:USE_READLINE] = false
  
  catch(:IRB_EXIT) do
    irb.eval_input
  end
end

RubyFu.menu_register("ruby-fu-console", RubyFu::RubyFuMenu)
