def module_template(base, methods)
  mod = Module.new
  methods.each do|name, args|
    args = args.join(',')
    mod.instance_eval """
      def self.#{name.gsub('-', '_')}(#{args})
        PDB['#{base}#{name}'].call(#{args})
      end
    """
  end
  
  return mod
end

def class_template(base, methods)
  klass = Class.new
  
  methods.each do|name, args|
    args_a = args.join(',')
    args_b = args.unshift('self').join(',')
    
    klass.instance_eval """
      def #{name.gsub('-', '_')}(#{args_a})
        PDB['#{base}#{name}'].call(#{args_b})
      end
    """
  end
  
  klass.instance_eval """
    def initialize(value)
      @value = value
    end
    
    def to_int
      @value
    end
    
    def to_str
      @value
    end
  """
  
  return klass
end

module Gimp
  Buffer = class_template('gimp-buffer-',
    [
      ['delete', []],
      ['get-bytes', []],
      ['get-height', []],
      ['get-image-type', []],
      ['get-width', []],
      ['rename', ['new_name']],
    ]
  )
  
  def Buffer.get_list(filter)
    PDB['gimp-buffer-get-list'].call(filter)
  end

  Context = module_template('gimp-context-',
    [
      ['get-background', []],
      ['get-brush', []],
      ['get-font', []],
      ['get-foreground', []],
      ['get-gradient', []],
      ['get-opacity', []],
      ['get-paint-method', []],
      ['get-paint-mode', []],
      ['get-palette', []],
      ['get-pattern', []],
      ['pop', []],
      ['push', []],
      ['set-background', ['bg']],
      ['set-brush', ['brush']],
      ['set-default-colors', []],
      ['set-font', ['font']],
      ['set-foreground', ['fg']],
      ['set-gradient', ['gradient']],
      ['set-opacity', ['opacity']],
      ['set-paint-method', ['method']],
      ['set-paint-mode', ['mode']],
      ['set-palette', ['palette']],
      ['set-pattern', ['pattern']],
      ['swap-colors', []],
    ]
  )
  
  #TODO
  #PDB
  #Progress
  
  Channel = class_template('gimp-channel-',
    [
      ['combine-masks', ['other', 'op', 'offx', 'offy']],
      ['copy', []],
      ['get-color', []],
      ['get-opacity', []],
      ['get-show-masked', []],
      ['set-color', ['color']],
      ['set-opacity', ['opacity']],
      ['set-show-masked', ['show']],
    ]
  )
  
  class Channel    
    def self.new(image, w, h, name, opacity, color)
      super PDB['gimp-channel-new'].call(image, w, h, name, opacity, color)
    end
    
    def self.new_from_component(image, component, name)
      #TODO FIXME
    end
  end
  
  Display = class_template('gimp-display-',
    [
      ['delete', []],
      ['get-window-handle', []],
    ]
  )
  
  class Display    
    def self.new(image)
      super PDB['gimp-display-new'].call(image)
    end
    
    def self.flush
      PDB['gimp-displays-flush'].call
    end
    
    def self.reconnect(old_image, new_image)
      PDB['gimp-displays-reconnect'].call(old_image, new_image)
    end
  end
  
  Drawable = class_template('gimp-drawable-',
    [
      ['bpp', []],
      ['bytes', []],
      ['delete', []],
      ['fill', ['fill_type']],
      ['foreground-extract', ['mode', 'mask']],
      ['get-image', []],
      ['get-linked', []],
      ['get-name', []],
      ['get-pixel', ['x', 'y']],
      ['get-tattoo', []],
      ['get-visible', []],
      ['has-alpha', []],
      ['height', []],
      ['image', []],
      ['is-channel', []],
      ['is-gray', []],
      ['is-indexed', []],
      ['is-layer', []],
      ['is-layer-mask', []],
      ['is-rgb', []],
      ['mask-bounds', []],
      ['mask-intersect', []],
      ['merge-shadow', ['undo']],
      ['offset', ['wrap', 'fill_type', 'x', 'y']],
      ['offsets', []],
      ['parasite-attach', ['parasite']],
      ['parasite-detach', ['name']],
      ['parasite-find', ['name']],
      ['parasite-list', []],
      ['set-linked', ['linked']],
      ['set-name', ['name']],
      ['set-pixel', ['x', 'y', 'num_channels', 'pixels']],
      ['set-tattoo', ['tattoo']],
      ['set-visible', ['visible']],
      ['sub-thumbnail', ['x', 'y', 'w', 'h', 'dest_h']],
      ['thumbnail', ['w', 'h']],
      #TODO transform functions
      ['type', []],
      ['type_with_alpha', []],
      ['update', ['x', 'y', 'w', 'h']],
      ['width', []],
    ]
  )
  
  Image = class_template('gimp-image-',
    [
      ['add-channel', ['channel', 'pos']],
      ['add-hguide', ['ypos']],
      ['add-layer', ['layer', 'pos']],
      ['add-layer-mask', ['layer', 'mask']],
      ['add-vectors', ['vectors', 'pos']],
      ['add-vguide', ['xpos']],
      ['base-type', []],
      ['clean-all', []],
      ['crop', ['w', 'h', 'x', 'y']],
      ['delete', []],
      ['delete-guide', ['guide']],
      ['duplicate', []],
      ['find-next-guide', ['guide']],
      ['flatten', []],
      ['flip', ['flip_type']],
      #TODO finish
    ]
  )
end