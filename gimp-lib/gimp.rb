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

$KCODE = "UTF8" unless RUBY_VERSION > "1.9.0"

require 'gimpext'
require 'pdb'

def _(str)
  Gimp.gettext(str)
end

def N_(str)
  str
end

def ruby2int_filter(value)
  case value
  when true   then 1
  when false  then 0
  when nil    then -1
  else value
  end
end

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
      when Class  then data.is_a? check
      when Symbol then data.respond_to? check
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
    
    def transform
      case type
      when PDB_DISPLAY
        Display.create(data)
      when PDB_IMAGE
        Image.create(data)
      when PDB_LAYER
        Layer.create(data)
      when PDB_CHANNEL
        Channel.create(data)
      when PDB_DRAWABLE
        Drawable.create(data)
      when PDB_VECTORS
        Vectors.create(data)
      else
        self.data
      end
    end
    
    def to_s
      "GimpParam #{EnumNames::PDBArgType[type]}: #{data}"
    end
  end
  
  class Rgb
    def marshal_dump
      [r, g, b, a]
    end
    
    def marshal_load(arr)
      new(*arr)
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
      data = Marshal.dump(obj)
      bytes = data.length
      PDB.gimp_procedural_db_set_data(key, bytes, data)
    end
  end
  
  autoload(:Drawable,  'gimp_oo_drawable.rb')
  autoload(:Brush,     'gimp_oo_brush.rb')
  autoload(:Channel,   'gimp_oo_channel.rb')
  autoload(:Display,   'gimp_oo_display.rb')
  autoload(:Gradient,  'gimp_oo_gradient.rb')
  autoload(:Image,     'gimp_oo_image.rb')
  autoload(:Layer,     'gimp_oo_layer.rb')
  autoload(:Palette,   'gimp_oo_palette.rb')
  autoload(:Vectors,   'gimp_oo_vectors.rb')

  autoload(:Context,   'gimp_oo_context.rb')
  autoload(:Edit,      'gimp_oo_edit.rb')
  autoload(:Progress,  'gimp_oo_progress.rb')
  autoload(:Selection, 'gimp_oo_selection.rb')
end
