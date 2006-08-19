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

require 'gimp.rb'

module RubyFu
  class CallError < Exception
  end
  
  class ResultError < Exception
  end
  
  class Cancel < Exception
  end
  
  class ParamDef < Gimp::ParamDef
    attr_reader :default, :subtype
    
    def self.method_missing(sym, *args)
      nargs = args.length
      if nargs == 3
        default = args.pop
        pdef = super(sym, *args)
        pdef.check(default)
        pdef.instance_variable_set(:@default, default)
        
        return pdef
      else
        return super
      end
    end
    
    def self.FONT(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :font)
      return value
    end
    
    def self.FILE(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :file)
      return value
    end
    
    def self.DIR(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :dir)
      return value
    end
    
    def self.PALETTE(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :palette)
      return value
    end
    
    def self.GRADIENT(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :gradient)
      return value
    end
    
    def self.PATTERN(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :pattern)
      return value
    end
    
    def self.BRUSH(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :brush)
      return value
    end
    
    def self.TOGGLE(*args)
      value = INT32(*args)
      value.instance_variable_set(:@subtype, :toggle)
      return value
    end
    
    def self.SPINNER(name, desc, default, range, step)
      value = FLOAT(name, desc, default)
      value.instance_variable_set(:@subtype, :spinner)
      value.instance_variable_set(:@range, range)
      value.instance_variable_set(:@step, step)
      return value
    end

    def self.SLIDER(name, desc, default, range, step)
      value = FLOAT(name, desc, default)
      value.instance_variable_set(:@subtype, :slider)
      value.instance_variable_set(:@range, range)
      value.instance_variable_set(:@step, step)
      return value
    end

    def self.ENUM(name, desc, default, enum)
      value = INT32(name, desc, default)
      value.instance_variable_set(:@subtype, :enum)
      value.instance_variable_set(:@enum, enum)
      return value
    end
    
    def self.LIST(name, desc, list)
      value = STRING(name, desc, list[0])
      value.instance_variable_set(:@subtype, :list)
      value.instance_variable_set(:@list, list)
      return value
    end

    def self.TEXT(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :text)
      return value
    end
  end

  class Procedure
    def initialize(*args, &func)
      @name, *rem = *args
      @blurb, @help, *rem = *rem
      @author, @copyright, *rem = *rem
      @date, *rem = *rem
      
      menulabel,*rem = *rem
      @menulabel = (menulabel.empty?) ? @name : menulabel
      @menupaths = []
      
      @imagetypes, *rem = *rem
      @params, @results, *rem = *rem
            
      @function = func
    end
    
    def add_menupath(path)
        type = case path
        when /<Image>/: :image
        when /<Toolbox>/: :toolbox
        end
        
        if @type and @type != type
          raise "Install locations don't match"
        else
          @type = type
        end
        
        @menupaths << path
    end
    
    def preparams
      case @type
      when :toolbox
        [Gimp::ParamDef.INT32('run-mode', 'Run mode')]
      when :image
        [
          Gimp::ParamDef.INT32('run-mode', 'Run mode'),
          Gimp::ParamDef.IMAGE('image', 'Input image'),
          Gimp::ParamDef.DRAWABLE('drawable', 'Input drawable'),
        ]
      else
        []
      end
    end
    
    def fullparams
      @fullparams ||= preparams + @params
    end
        
    def query
      Gimp.install_procedure(
        @name,
        @blurb,
        @help,
        @author,
        @copyright,
        @date,
        @menulabel,
        @imagetypes,
        Gimp::PLUGIN,
        (fullparams.empty? ? nil : fullparams),
        (@results.empty? ? nil : @results)
      )
      
      @menupaths.each do|menupath|
        PDB.gimp_plugin_menu_register(@name, menupath)
      end
    end
    
    def default_args
      defArgs = @params.collect do|pdef|
        pdef.default if pdef.respond_to? :default
      end
    end
    
    def get_interactive_args
      return [] if @params.empty?
      
      args = RubyFu.dialog(@menulabel, @name, @params)
      raise Cancel unless args
      
      Gimp::Shelf[@name + ':last_params'] = args
      args
    end
    
    def get_last_args
      args = Gimp::Shelf[@name + ':last_params']
      
      if args
        args
      else
        default_args
      end
    end
    
    def run_with_args(args)
      nargs = args.length
      nparams = fullparams.length
      unless nargs == nparams
        message = "Wrong number of arguments. (#{nargs} for #{nparams})"
        raise(CallError, message)
      end
      
      return @function.call(*args)
    end
    
    def run(*args)
      if @type
        runMode = args[0].data
      else
        runMode = Gimp::RUN_NONINTERACTIVE
      end
      
      extra_args = case runMode
      when Gimp::RUN_INTERACTIVE: get_interactive_args
      when Gimp::RUN_WITH_LAST_VALS: get_last_args
      else []
      end
      
      args = args.zip(fullparams).collect do|arg, param|
        raise(CallError, "Bad argument") unless arg.type == param.type
        next arg.transform
      end
      
      values = run_with_args(args + extra_args)
            
      if values == nil or @results.empty?
        values = []
      else
        *values = *values
      end

      nvalues = values.length
      nresults = @results.length
      unless nvalues == nresults
        message = "Wrong number of return values. (#{nvalues} for #{nresults})"
        raise(ResultError, message)
      end
      
      begin
        values = values.zip(@results).collect do|value, result|
          value = ruby2int_filter(value)
          result.check(value)
          Gimp::Param.new(result.type, value)
        end
      rescue TypeError
        message = "Procedure return value type check failed: #{$!.message}"
        raise(TypeError, message)
      end
      
      return values
    end
  end
  
  @@procedures = {}
  @@menubranches = []
  
  def register(
    name,
    blurb,
    help,
    author,
    copyright,
    date,
    menupath,
    imagetypes,
    params,
    results,
    &function
  )
    proc = Procedure.new(
      String(name),
      String(blurb),
      String(help),
      String(author),
      String(copyright),
      String(date),
      String(menupath),
      String(imagetypes),
      Array(params),
      Array(results),
      &function
    )
    
    @@procedures[name] = proc
  end
  module_function :register
  
  def menu_register(name, path)
    proc = @@procedures[name]
    proc.add_menupath(path)
  end
  module_function :menu_register
  
  def menu_branch_register(path, name)
    @@menubranches << [path, name]
    File.join(path, name)
  end
  module_function :menu_branch_register
  
  def self.query
    @@procedures.each_value{|proc| proc.query}
    @@menubranches.each do|path, name|
      PDB.gimp_plugin_menu_branch_register(path, name)
    end
  end
  
  def self.run(name, *args)
    begin
      proc = @@procedures[name]
      
      values = proc.run(*args)
      values.unshift Gimp::Param.STATUS(Gimp::PDB_SUCCESS)
      
      return values
    rescue CallError
      PDB.gimp_message("A calling error has occured: #$!.message")
      return [Gimp::Param.STATUS(Gimp::PDB_CALLING_ERROR)]
    rescue Cancel
      return [Gimp::Param.STATUS(Gimp::PDB_CANCEL)]
    rescue Exception
      message = "A #{$!.class} has occured: #{$!.message}\n#{$@.join("\n")}"
      PDB.gimp_message message
      return [Gimp::Param.STATUS(Gimp::PDB_EXECUTION_ERROR)]
    end
  end
  
  PLUG_IN_INFO = Gimp::PlugInInfo.new(
    nil,
    nil,
    method(:query),
    method(:run)
  )
  
  def self.main
    Gimp.main(PLUG_IN_INFO)
  end
  
  RubyFuMenu = '<Toolbox>/Xtns/Languages/Ruby-Fu'
  ExamplesMenu = '<Toolbox>/Xtns/Languages/Ruby-Fu/Examples'
end

END {
  RubyFu.main
}