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

module RubyFu
  class Procedure
    attr_reader :type
  end

  def self.test_proc(procname, drawable)
    proc = @@procedures[procname]
    
    params = proc.fullparams
    case proc.type
    when :toolbox:
      proc.run(Param.INT32(RUN_INTERACTIVE))
    when :image:
      image = Param.IMAGE(PDB.gimp_drawable_get_image(drawable))
      drawable = Param.DRAWABLE(drawable)
      proc.run(Param.INT32(RUN_INTERACTIVE), image, drawable)
    else
      args = RubyFu.dialog("Testing #{procname}", procname, params)
      proc.run(*args)
    end
  end
end

help_string = _(
  "The procedure argument should be the name of the " +
  "procedure defined in the file you wish to run. " +
  "The drawable argument is only needed if the procedure "+
  "is image based."
)

register(
  :name      => "ruby-fu-run-file",
  :blurb     => _("Runs a Ruby-Fu script without requiring you to install it."), #blurb
  :help      => help_string, #help
  :author    => "Scott Lembcke", #author
  :copyright => "Scott Lembcke", #copyright
  :date      => "2006", #date
  :menupath  => _("Run File"), #menupath
  :params    => [
    ParamDef.FILE("file", _("File")),
    ParamDef.STRING("procedure", _("Procedure name"), "ruby-fu-"),
    ParamDef.DRAWABLE("drawable", _("Drawable (if needed)")),
  ]
) do |run_mode, filename, procname, drawable|
  Shelf["ruby-fu-last-run-file"] = [filename, procname, drawable]

  load(filename)
  RubyFu.test_proc(procname, drawable)
end

menu_register("ruby-fu-run-file", RubyFuMenu)


register(
  :name => "ruby-fu-rerun-file", #procedure name
  :blurb => _("Reruns the last file ran using Runfile"), #blurb
  :author => "Scott Lembcke", #author
  :copyright => "Scott Lembcke", #copyright
  :date => "2006", #date
  :menupath => _("Rerun File")
) do |run_mode, filename, procname|

  last = Shelf["ruby-fu-last-run-file"]
  
  if last
    PDB.ruby_fu_run_file(*last)
  else
    Gimp.message _("No previous file to run")
  end
end

menu_register("ruby-fu-rerun-file", RubyFuMenu)
