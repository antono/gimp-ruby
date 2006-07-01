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
