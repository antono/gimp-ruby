require 'gimpext'
require 'pdb'
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
      :PARASITE => Gimp::Parasite,
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
  
  class Rgb
    def _dump(level)
      Marshal.dump([r, g, b, a])
    end
    
    def _load(str)
      new(*Marshal.load(str))
    end
    
    def eql?(other)
      return false unless other.is_a? Rgb
      
      unless r == other.r
             g == other.g
             b == other.b
             a == other.a
        return false
      end
      
      return true
    end
    
    alias_method :==, :eql?
    
    def to_s
      "RGB<r=#{r}, g=#{g}, b=#{b}, a=#{a}>"
    end
  end
  
  def Color(*args)
    Rgb.new(*args)
  end
  module_function :Color
  
  module Shelf
    def self.[](key)
      begin
        bytes, data = PDB.gimp_procedural_db_get_data(key)
        return Marshal.load(data)
      rescue PDB::ExecutionError
        return nil
      end
    end
    
    def self.[]=(key, obj)
      puts 'setting'
      data = Marshal.dump(obj)
      bytes = data.length
      PDB.gimp_procedural_db_set_data(key, bytes, data)
    end
  end
end
