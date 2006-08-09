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

module Gimp
  module GimpOO
    module ProcList
      module_function
      def each_proc(prefix)
        prefix_range = prefix.length..-1
        get_proc_list(prefix).each do|proc_name|
          sym = proc_name[prefix_range].gsub('-','_').to_sym
          yield(sym, proc_name)
        end
      end
  
      def get_proc_list(prefix)
        num, list =PDB['gimp-procedural-db-query'].call(prefix, *(['']*6))
        return list
      end
    end

    class ClassTemplate
      class << self
        def add_constructor(name, proc_name)
          class_eval """
            def self.#{name}(*args)
              PDB['#{proc_name}'].call(*args)
            end
          """
        end
    
        def add_method(name, proc_name)
          class_eval """
            def #{name}(*args)
              PDB['#{proc_name}'].call(self, *args)
            end
          """
        end
    
        def add_class_method(name, proc_name)
          class_eval """
            def self.#{name}(*args)
              PDB['#{proc_name}'].call(*args)
            end
          """
        end
    
        def add_methods(prefix, class_prefix)
          ProcList.each_proc(prefix) do|sym, proc_name|
            if proc_name =~ /new/
              add_constructor(sym, proc_name)
            else
              add_method(sym, proc_name)
            end
          end
      
          if class_prefix
            ProcList.each_proc(class_prefix) do|sym, proc_name|
              add_class_method(sym, proc_name)
            end
          end
        end
    
        def template(prefix, class_prefix, super_class = self)
          klass = Class.new(super_class)
          klass.add_methods(prefix, class_prefix)
          
          return klass
        end
    
        alias_method :create, :new
      end
  
      def initialize(value)
        @self = value
      end

      def to_int
        Integer(@self)
      end
  
      def to_str
        String(@self)
      end
    end

    module ModuleTemplate
      def self.template(prefix)
        mod = Module.new
        ProcList.each_proc(prefix) do|sym, proc_name|
          mod.module_eval """
            def self.#{sym}(*args)
              PDB['#{proc_name}'].call(*args)
            end
          """
        end
    
        return mod
      end #def
    end #module
  end #module
end #module
