require 'gimpext'
#require 'gimp_oo.rb'
require 'rubyfu.rb'

module Gimp
  def message(*messages)
    messages.each do|message|
      PDB.gimp_message(message.to_s)
    end
  end
  
  module_function :message
  
  module ParamTypes
    CheckType = {
      :INT32 => :to_int,
      :INT16 => :to_int,
      :INT8 => :to_int,
      :FLOAT => :to_f,
      :STRING => :to_str,
      :INT32ARRAY => :to_ary,
      :INT16ARRAY => :to_ary,
      :INT8ARRAY => :to_str,
      :FLOATARRAY => :to_ary,
      :STRINGARRAY => :to_ary,
      :COLOR => Gimp::Rgb,
      :DISPLAY => :to_int,
      :IMAGE => :to_int,
      :LAYER => :to_int,
      :CHANNEL => :to_int,
      :DRAWABLE => :to_int,
      :SELECTION => :to_int,
      :BOUNDARY => :to_int,
      :VECTORS => :to_int,
  #    add_type 'PARASITE', NilClass
      :STATUS => :to_int,
    }
    
    INT2TYPE = Hash.new
    EnumNames::PDBArgType.each do|key, value|
      INT2TYPE[key] = value.gsub('PDB_', '').to_sym
    end
    
    def self.check_type(sym, data)
      check = CheckType[sym]
      
      good_type = case check
      when Class: data.is_a? check
      when Symbol: data.respond_to? check
      end
      
      unless good_type
        message = "#A #{sym} cannot be created from a #{data.class}"
        raise(TypeError, message)
      end
    end
    
    def self.check_method(sym, args, nargs)
      return false unless CheckType.member? sym
      
      arglen = args.length
      unless arglen == nargs
        message = "Wrong number of arguments. (#{arglen} for #{nargs})"
        raise(ArgumentError, message)
      end
      
      return true
    end
  end
  
  class ParamDef
    def self.method_missing(sym, *args)
      super unless ParamTypes.check_method(sym, args, 2)
      
      name, desc = *args
      return new(Gimp.const_get("PDB_#{sym}".to_sym), name, desc)
    end
    
    def check(value)
      name = EnumNames::PDBArgType[type]
      sym = name.sub('PDB_', '').to_sym
            
      ParamTypes.check_type(sym, value)
    end
  end
  
  class Param    
    def self.method_missing(sym, *args)
      super unless ParamTypes.check_method(sym, args, 1)
      
      data = *args
      ParamTypes.check_type(sym, data)
      return new(Gimp.const_get("PDB_#{sym}".to_sym), data)
    end
    
    def to_s
      "GimpParam #{EnumNames::PDBArgType[type]}: #{data}"
    end
  end
end

module PDB
  class PDBException < Exception
  end

  class NoProcedure < PDBException
    def initialize(name)
      @name = name
    end
    
    def message
      "'#@name' is not in the PDB"
    end
  end
  
  class CallError < PDBException
    def initialize(name, status)
      @name = name
      @status = Gimp::EnumNames::PDBStatusType[status]
    end
    
    def message
      "#@name exited with status #@status"
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
    
    def convert_args(args, paramdefs)
      arglen = args.length
      prmlen = paramdefs.length
      unless arglen == prmlen
        message = "Wrong number of parameters. #{arglen} for #{prmlen}"
        raise(ArgumentError, message)
      end
      
      begin
        result = args.zip(paramdefs).collect do|arg, paramdef|
          paramdef.check(arg)
          Gimp::Param.new(paramdef.type, arg)
        end
      rescue TypeError
        message = "Bad Argument: #{$!.message}"
        raise(TypeError, message)
      end
    end
    
    def call(*args)
      params = convert_args(args, @args)
      values = Gimp.run_procedure(@name, params)
      
      status = values.shift.data
      raise CallError.new(@name, status) unless status == Gimp::PDB_SUCCESS
      
      return *values.collect{|param| param.data}
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
  
  def self.[](name)
    Procedure.new(name)
  end
  
  module Access
    def self.method_missing(sym, *args)
      # begin
        Procedure.new(sym.to_s.gsub('_', '-')).call(*args)
      # rescue NoProcedure
      #   warn $!.message
      #   super
      # end
    end
    
    def method_missing(sym, *args)
      Access.method_missing(sym, *args)
    end
  end
  
  def self.method_missing(sym, *args)
    Access.method_missing(sym, *args)
  end
end
