require 'gimpext'
require 'gimp_oo.rb'
require 'rubyfu.rb'

module Gimp  
  class ParamDef
    def self.add_type(method_name)
      class_eval """
        def self.#{method_name}(name, desc)
          ParamDef.new(Gimp::PDB_#{method_name}, name, desc)
        end
      """
    end
      
    add_type 'INT32'
    add_type 'INT16'
    add_type 'INT8'
    add_type 'FLOAT'
    add_type 'STRING'
    add_type 'INT32ARRAY'
    add_type 'INT16ARRAY'
    add_type 'INT8ARRAY'
    add_type 'FLOATARRAY'
    add_type 'STRINGARRAY'
    add_type 'COLOR'
    add_type 'REGION'
    add_type 'DISPLAY'
    add_type 'IMAGE'
    add_type 'LAYER'
    add_type 'CHANNEL'
    add_type 'DRAWABLE'
    add_type 'SELECTION'
    add_type 'BOUNDARY'
    add_type 'VECTORS'
    add_type 'PARASITE'
    add_type 'STATUS'
  end
  
  class Param
    def self.duck_type(method_name, respond)
      class_eval """
        def self.#{method_name}(data)
          message = \"Cannot create a #{method_name} from a \#{data.class}\"
          raise(ArgumentError, message) unless data.respond_to? #{respond}
          Param.new(Gimp::PDB_#{method_name}, data)
        end
      """
    end
    
    def self.add_type(method_name, klass)
      class_eval """
        def self.#{method_name}(data)
          message = \"Cannot create a #{method_name} from a \#{data.class}\"
          raise(ArgumentError, message) unless data.is_a? #{klass}
          Param.new(Gimp::PDB_#{method_name}, data)
        end
      """
    end
    
    duck_type 'INT32', ':to_int'
    duck_type 'INT16', ':to_int'
    duck_type 'INT8', ':to_int'
    duck_type 'FLOAT', ':to_f'
    duck_type 'STRING', ':to_str'
    duck_type 'INT32ARRAY', ':to_ary'
    duck_type 'INT16ARRAY', ':to_ary'
    duck_type 'INT8ARRAY', ':to_ary'
    duck_type 'FLOATARRAY', ':to_ary'
    duck_type 'STRINGARRAY', ':to_ary'
    add_type 'COLOR', Gimp::Rgb
#    add_type 'REGION', NilClass
    duck_type 'DISPLAY', ':to_int'
    duck_type 'IMAGE', ':to_int'
    duck_type 'LAYER', ':to_int'
    duck_type 'CHANNEL', ':to_int'
    duck_type 'DRAWABLE', ':to_int'
    duck_type 'SELECTION', ':to_int'
    duck_type 'BOUNDARY', ':to_int'
    duck_type 'VECTORS', ':to_int'
#    add_type 'PARASITE', NilClass
    duck_type 'STATUS', ':to_int'
  end
end

module PDB
  class Execption
  end

  class NoProcedure < Exception
    def initialize(name)
      @name = name
    end
    
    def message
      "'#@name' is not in the PDB"
    end
  end
  
  class CallError < Exception
    def initialize(name, status)
      @name, @status = name, status
    end
    
    def message
      "#{@name} exited with status #{status}"
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
      
      result = args.zip(paramdefs).collect do|arg, paramdef|
        Gimp::Param.new(paramdef.type, arg)
      end
    end
    
    def call(*args)
      params = convert_args(args, @args)
      values = Gimp.run_procedure(@name, params)
      
      status = values.shift
      raise CallError.new(@name, status) unless status == Gimp::PDB_SUCCESS
      
      case values.length
      when 0: return nil
      when 1: return values.first
      else return values
      end
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
  end
  
  def self.[](name)
    Procedure.new(name)
  end
  
  module Access
    def self.method_missing(sym, *args)
      begin
        Procedure.new(sym.to_s.gsub('_', '-')).call(*args)
      rescue NoProcedure
        warn $!.message
        super
      end
    end
    
    def method_missing(sym, *args)
      Access.method_missing(sym, *args)
    end
  end
  
  def self.method_missing(sym, *args)
    Access.method_missing(sym, *args)
  end
end
