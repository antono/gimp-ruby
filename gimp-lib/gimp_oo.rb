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
      end
    end
    
    Gimp::Drawable = ClassTemplate.template('gimp-drawable-', nil)
    Gimp::Brush = ClassTemplate.template('gimp-brush-', 'gimp-brushes-')
    Gimp::Channel = ClassTemplate.template('gimp-channel-', nil, Gimp::Drawable)
    Gimp::Display = ClassTemplate.template('gimp-display-', 'gimp-displays-')
    Gimp::Gradient = ClassTemplate.template('gimp-gradient-', 'gimp-gradients-')
    Gimp::Image = ClassTemplate.template('gimp-image-', nil)
    Gimp::Layer = ClassTemplate.template('gimp-layer-', nil, Gimp::Drawable)
    Gimp::Palette = ClassTemplate.template('gimp-palette-', 'gimp-palettes-')
    Gimp::Vectors = ClassTemplate.template('gimp-vectors-', nil)

    Gimp::Context = ModuleTemplate.template('gimp-context-')
    Gimp::Edit = ModuleTemplate.template('gimp-edit-')
    Gimp::Fonts = ModuleTemplate.template('gimp-fonts-')
    Gimp::ProceduralDB = ModuleTemplate.template('gimp-procedural-db-')
    Gimp::Progress = ModuleTemplate.template('gimp-progress-')
    Gimp::Selection = ModuleTemplate.template('gimp-selection-')
  end
  
  class Image
    add_class_method('list', 'gimp-image-list')
  
    alias_method :old_undo_group_start, :undo_group_start
    def undo_group
      old_undo_group_start
      if block_given?
        begin
          yield
        ensure
          undo_group_end
        end
      end
    end
    
    alias_method :old_undo_disable, :undo_disable
    def undo_disable
      old_undo_disable
      if block_given?
        begin
          yield
        ensure
          undo_enable
        end
      end #if
    end #meth
  end #class
  
  module Context
    class << self
      alias_method :old_push, :push
      def push
        old_push
        if block_given?
          begin
            yield
          ensure
            pop
          end
        end #if
      end #meth
    end #class
  end #module
end #module

module Gimp
  class Param
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
      when PDB_VECTORS
        Vectors.create(data)
      else
        self.data
      end
    end
  end
end
