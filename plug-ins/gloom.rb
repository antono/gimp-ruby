require 'rubyfu'

Size = 256

include Gimp
include RubyFu

register(
  'ruby-fu-gloom', #procedure name
  '', #blurb
  '', #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Image>/Ruby-Fu/Gloom', #menupath
  '*', #image types
  [
    ParamDef.SLIDER('radius', 'Radius', 5.0, 0..50, 0.1),
    ParamDef.SLIDER('amount', 'Amount', 100, 0..100, 1),
  ], #params
  [] #results
) do|run_mode, image, drawable, radius, amount|
  include PDB::Access
  
  image.undo_group do
    gloom = Layer.new_from_drawable(drawable, image)
    image.add_layer(gloom, -1)
    gimp_drawable_set_name(gloom, 'gloom')
    gloom.set_mode(DARKEN_ONLY_MODE)
    
    plug_in_gauss_iir2(RUN_NONINTERACTIVE, image, gloom, radius, radius)
  end
  
  Display.flush
  []
end
