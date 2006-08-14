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

module RubyFu
  class Procedure
    attr_reader :fullparams, :type
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
    when :internal
      args = RubyFu.dialog("Testing #{procname}", procname, params)
      proc.run(*args)
    end
  end
end

register(
  'ruby-fu-runfile', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Run File', #menupath
  nil, #image types
  [
    ParamDef.FILE('file', 'File'),
    ParamDef.STRING('procedure', 'Procedure name', 'ruby-fu-'),
    ParamDef.DRAWABLE('drawable', 'Drawable (if needed)'),
  ], #params
  [] #results
) do|run_mode, filename, procname, drawable|
  Shelf['ruby-fu-last-runfile'] = [filename, procname, drawable]

  load(filename)
  RubyFu.test_proc(procname, drawable)
end

register(
  'ruby-fu-rerunfile', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Rerun File', #menupath
  nil, #image types
  [], #params
  [] #results
) do|run_mode, filename, procname|
  last = Shelf['ruby-fu-last-runfile']
  
  if last
    PDB.ruby_fu_runfile(*last)
  else
    Gimp.message 'No previous file to run'
  end
end
