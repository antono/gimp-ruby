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

module PDB
  class PDBException < RuntimeError
  end

  class NoProcedure < PDBException
    def initialize(name)
      @name = name
    end
    
    def message
      "'#@name' is not in the PDB"
    end
  end
  
  class ExecutionError < PDBException
    attr_reader :message
    def initialize(proc_name)
      @message = "#{proc_name} returned an execution error."
    end
  end
  
  class CallingError < PDBException
    attr_reader :message
    def initialize(proc_name)
      @message = "#{proc_name} returned a calling error."
    end
  end
  
  class Procedure
    attr_reader :name, :blurb, :help, :author, :copyright, :date
    attr_reader :proc_type, :args, :return_vals
    
    @@cache = {}
    
    def self.new(name)
      @@cache[name] = super unless @@cache.include? name
      return @@cache[name]
    end
    
    def initialize(name)
      @name = name
      
      values = Gimp.procedural_db_proc_info(name)
      raise(NoProcedure, name) unless values
      
      @blurb = values.shift
      @help = values.shift
      @author = values.shift
      @copyright = values.shift
      @date = values.shift
      @proc_type = values.shift
      @args = values.shift
      @return_vals = values.shift
    end
    
    def convert_args(args)
      arglen = args.length
      prmlen = @args.length
      unless arglen == prmlen
        message = "Wrong number of parameters. #{arglen} for #{prmlen}"
        raise(ArgumentError, message)
      end
      
      begin
        result = args.zip(@args).collect do|arg, paramdef|
          arg = ruby2int_filter(arg)
          paramdef.check(arg)
          Gimp::Param.new(paramdef.type, arg)
        end
      rescue TypeError
        message = "Bad Argument: #{$!.message}"
        raise(TypeError, message)
      end
    end
    
    def convert_return_values(values)
      case values.shift.data
      when Gimp::PDB_CALLING_ERROR    then raise(CallingError, @name)
      when Gimp::PDB_EXECUTION_ERROR  then raise(ExecutionError, @name)
      end
      
      values.collect{|param| param.transform}
    end
    
    def call(*args)
      if @args[0] and @args[0].name == 'run-mode'
        args.unshift(Gimp::RUN_NONINTERACTIVE)
      end
      
      if PDB.verbose
        arg_str = args.collect{|arg| arg.inspect}.join(', ')
        puts "PDB call: #@name(#{arg_str})"
      end

      params = convert_args(args)      
      return *convert_return_values(Gimp.run_procedure(@name, params))
    end
    
    def to_s
      [
        "       name: #@name",
        "      blurb: #@blurb",
        "       help: #@help",
        "     author: #@author",
        "  copyright: #@copyright",
        "       date: #@date",
        "  proc_type: #@proc_type",
        "       args: #@args",
        "return_vals: #@return_vals",
      ].join("\n")
    end
    
    def to_proc
      lambda do|*args|
        self.call(*args)
      end
    end
  end
  
  class << self
    attr_accessor :verbose
    @verbose = false
    
    def [](name)
      Procedure.new(name)
    end
    
    def call_interactive(name, image = nil, drawable = nil)
      proc = Procedure.new(name)
      arg = proc.args[0]
      
      if arg and arg.name == 'run-mode'
        arg1 = proc.args[1]
        arg2 = proc.args[2]
        args = [Gimp::Param.INT32(Gimp::RUN_INTERACTIVE)]
        
        if arg1 and arg2 and arg1.name == 'image' and arg2.name == 'drawable'
          args += [Gimp::Param.IMAGE(image), Gimp::Param.DRAWABLE(drawable)]
        end
        
        return *proc.convert_return_values(Gimp.run_procedure(name, args))
      else
        raise 'poop'
      end
    end
  end
  
  module Access
    def method_missing(sym, *args)
      begin
        Procedure.new(sym.to_s.gsub('_', '-')).call(*args)
      rescue NoProcedure
        warn $!.message
        super
      end
    end
    module_function :method_missing
  end
  
  extend Access
end
