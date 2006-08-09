require 'rubyfu'

include Gimp
include RubyFu

register(
  'ruby-fu-hello-world', #procedure name
  'Display a message.', #blurb
  'Makes a new image to display a message', #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Hello World', #menupath
  nil, #image types
  [
    ParamDef.STRING('message', 'Message', 'Hello World'),
    ParamDef.FLOAT('size', 'Font Size (Pixels)', 48),
    ParamDef.FONT('font', 'Font', 'verdana'),
    ParamDef.COLOR('color', 'Color', Color(0.0, 0.0, 0.0)),
  ], #params
  [
    ParamDef.IMAGE('image', 'Image'),
  ] #results
) do|run_mode, message, size, font, color|
  include PDB::Access
  
  bounds = gimp_text_get_extents_fontname(message, size, PIXELS, font)
  width, height, ascent, descent = *bounds
  
  img = Image.new(width, height, RGB)
  layer = Layer.new(img, width, height, RGB_IMAGE, "hello", 100, NORMAL_MODE)
  layer.fill(WHITE_FILL)
  img.add_layer(layer, -1)

  Context.push do
    Context.set_foreground(color)

    text_layer = gimp_text_fontname(img, layer, 0, 0, message, 0, true, size, PIXELS, font)
    gimp_floating_sel_anchor(text_layer)
  end
  
  Display.new(img)

  [img]
end
