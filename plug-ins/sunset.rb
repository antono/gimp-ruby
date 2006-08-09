require 'rubyfu'

include Gimp
include RubyFu

register(
  'ruby-fu-sunset', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  'Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Sunset', #menupath
  nil, #image types
  [
    ParamDef.INT32('width', 'Width', 640),
    ParamDef.INT32('height', 'Height', 480),
  ], #params
  [
    ParamDef.IMAGE('image', 'Image'),
  ] #results
) do|run_mode, w, h|
  include PDB::Access

  image = Image.new(w, h, RGB)
  
  sunset = Layer.new(image, w, h, RGB_IMAGE, 'sunset', 100, NORMAL_MODE)
  image.add_layer(sunset, -1)
    
  fg = Color(0.025, 0.000, 0.219)
  bg = Color(1.000, 0.870, 0.000)
  
  Context.set_foreground(fg)
  Context.set_background(bg)
  Context.set_gradient('FG to BG (HSV counter-clockwise)')

  gimp_rect_select(image, 0, 0, w, h/3, CHANNEL_OP_REPLACE, false, 0.0)
  Edit.blend(sunset, CUSTOM_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, 0, 0, h/3)
  gimp_rect_select(image, 0, h/3, w, h, CHANNEL_OP_REPLACE, false, 0.0)
  Edit.blend(sunset, CUSTOM_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, h, 0, h/3)
  Selection.none(image)

  waves = Layer.new(image, w, h, RGB_IMAGE, 'waves', 100, NORMAL_MODE)
  image.add_layer(waves, -1)
  plug_in_solid_noise(RUN_NONINTERACTIVE, image, waves, false, false, rand(10_000), 15, 1.5, 16)    
  
  Context.set_foreground(Color(0.5, 0.5, 0.5))
  Edit.blend(waves, FG_TRANSPARENT_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE, FALSE, FALSE, 1, 0, TRUE, 0, h/3, 0, h)
  plug_in_displace(RUN_NONINTERACTIVE, image, sunset, 0, h/4, false, true, -1, waves, 1)
  
  plug_in_bump_map(RUN_NONINTERACTIVE, image, sunset, waves, 270, 50, 6, 0, 0, 0, 111, true, false, 0)
  image.remove_layer(waves)

  gimp_rect_select(image, 0, h/3, w, h, CHANNEL_OP_REPLACE, false, 0.0)
  gimp_levels(sunset, HISTOGRAM_VALUE, 0, 255, 0.65, 0, 255)
  Selection.none(image)

  Display.new(image)

  [image]
end
