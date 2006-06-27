#!/usr/bin/env ruby

require "gimp"
include Gimp
include RubyFu

include PDB::Access

def apply_cool_metal_logo_effect(img, logo_layer, size, bg_color, gradient)

  feather    = size / 5
  smear      = 7.5
  period     = size / 3
  amplitude  = size / 40
  shrink     = size / 30 + 1
  depth      = size / 20
  width      = gimp_drawable_width(logo_layer)
  height     = gimp_drawable_height(logo_layer)
  img_width  = width + 0.15 * height + 10
  img_height = 1.85 * height + 10
  # bg_layer = Layer::new(img, img_width, img_height, RGB_IMAGE, "Background")
  bg_layer = gimp_layer_new(img, img_width, img_height, RGB_IMAGE, "Background", 100.0, NORMAL_MODE)
  # shadow_layer = Layer::new(img, img_width, img_height, RGBA_IMAGE, "Shadow")
  shadow_layer = gimp_layer_new(img, img_width, img_height, RGBA_IMAGE, "Shadow", 100.0, NORMAL_MODE)
  # reflect_layer = Layer::new(img, img_width, img_height, RGBA_IMAGE, "Reflection")
  reflect_layer = gimp_layer_new(img, img_width, img_height, RGBA_IMAGE, "Reflection", 100.0, NORMAL_MODE)

  gimp_context_push
  begin
    # img.resize(img_width, img_height, 0, 0)
    gimp_image_resize(img, img_width, img_height, 0, 0)
    # img.add_layer(bg_layer, 1)
    gimp_image_add_layer(img, bg_layer, 1)
    # img.add_layer(reflect_layer, 1)
    gimp_image_add_layer(img, reflect_layer, 1)
    # img.add_layer(shadow_layer, 1)
    gimp_image_add_layer(img, shadow_layer, 1)
    # logo_layer.preserve_trans = true
    gimp_layer_set_lock_alpha(logo_layer, 1)
        
    
    gimp_context_set_background(bg_color)
    gimp_edit_fill(bg_layer, BACKGROUND_FILL)
    gimp_edit_clear(reflect_layer)
    gimp_context_set_background(Gimp::Rgb.new(0.0, 0.0, 0.0))
    gimp_edit_fill(shadow_layer, BACKGROUND_FILL)
    
    gimp_context_set_gradient(gradient)
    gimp_edit_blend(logo_layer, CUSTOM_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE,
    	       0, 0, 0, 0, 0, 0, 0, 0, height + 5)
    gimp_rect_select(img, 0, height / 2 - feather, img_width, feather * 2,
    		     CHANNEL_OP_REPLACE, 0, 0)
    plug_in_gauss_iir(RUN_NONINTERACTIVE,
    		      img, logo_layer, smear, 1, 1)
    gimp_selection_none(img)
    plug_in_ripple(RUN_NONINTERACTIVE, img, logo_layer,
    		   period, amplitude, 1, 0, 1, 1, 0)
    # logo_layer.translate(5, 5)
    gimp_layer_translate(logo_layer, 5, 5)
    # logo_layer.resize(img_width, img_height, 5, 5)
    gimp_layer_resize(logo_layer, img_width, img_height, 5, 5)
      
    gimp_selection_layer_alpha(logo_layer)
    channel = gimp_selection_save(img)
    gimp_selection_shrink(img, shrink)
    gimp_selection_invert(img)
    plug_in_gauss_rle(RUN_NONINTERACTIVE,
    		      img, channel, feather, 1, 1)
    gimp_selection_layer_alpha(logo_layer)
    gimp_selection_invert(img)
    gimp_context_set_background(Gimp::Rgb.new(0.0, 0.0, 0.0))
    gimp_edit_fill(channel, BACKGROUND_FILL)
    gimp_selection_none(img)
      
    plug_in_bump_map(RUN_NONINTERACTIVE, img, logo_layer, channel,
    		     135, 45, depth, 0, 0, 0, 0, 0, 0, 0)
    
    gimp_selection_layer_alpha(logo_layer)
    fs = gimp_selection_float(shadow_layer, 0, 0)
    gimp_edit_clear(shadow_layer)
    gimp_drawable_transform_perspective_default(
         fs, 5 + 0.15 * height, height - 0.15 * height,
    		     5 + width + 0.15 * height, height - 0.15 * height,
    		     5.0, Float(height), 5 + width, Float(height),
    		     0, 0)
    # fs.anchor
    gimp_floating_sel_anchor(fs)
    plug_in_gauss_rle(RUN_NONINTERACTIVE,
    		      img, shadow_layer, smear, 1, 1)
        
    # gimp_rect_select(img, 5, 5, width, height, CHANNEL_OP_REPLACE, 0, 0)
    gimp_edit_copy(logo_layer)
    fs = gimp_edit_paste(reflect_layer, 1)
    gimp_floating_sel_anchor(fs)
    gimp_drawable_transform_scale_default(
        reflect_layer, 0, 0, width, height * 0.85, 0, 0)
    gimp_drawable_transform_flip_simple(
            reflect_layer, 1, 1, 0, 0)
    # reflect_layer.set_offsets(5, 3 + height)
    gimp_layer_set_offsets(reflect_layer, 5, 3 + height)
      
    # layer_mask = reflect_layer.create_mask(WHITE_MASK)
    layer_mask = gimp_layer_create_mask(reflect_layer, ADD_WHITE_MASK)
    # img.add_layer_mask(reflect_layer, layer_mask)
    gimp_layer_add_mask(reflect_layer, layer_mask)
    gimp_context_set_foreground(Gimp::Rgb.new(1.0, 1.0, 1.0))
    gimp_context_set_background(Gimp::Rgb.new(0.0, 0.0, 0.0))
    gimp_edit_blend(layer_mask, FG_BG_RGB_MODE, NORMAL_MODE, GRADIENT_LINEAR, 100, 0, REPEAT_NONE,
    	       0, 0, 0, 0, 0, 0, - height / 2, 0, height)
        
    # img.remove_channel(channel)
    gimp_image_remove_channel(img, channel)
  ensure
    gimp_context_pop
  end
end


def ruby_fu_cool_metal_logo_alpha(run_mode, img, logo_layer, size = 100, bg_color = Gimp::Rgb.new(1.0, 1.0, 1.0), gradient = "Horizon_1")
  gimp_image_undo_group_start(img)
    apply_cool_metal_logo_effect(img, logo_layer, size, bg_color, gradient)
  gimp_image_undo_group_end(img)
  gimp_displays_flush
end


RubyFu.register(
  "ruby-fu-cool-metal-logo-alpha",
  "Metallic logos with reflections and perspective shadows",
  "Metallic logos with reflections and perspective shadows",
  "Spencer Kimball & Rob Malda",
  "Spencer Kimball & Rob Malda",
  "1997",
  "<Image>/Ruby-Fu/Alpha to Logo/Cool Metal...",
  "RGBA",
  [
    ParamDef.INT32("effect_size", "Effect Size (pixels)"),
    ParamDef.COLOR("bg_color", "Background Color"),
    ParamDef.STRING("gradient",  "Gradient"),
  ],
  nil,
  &method(:ruby_fu_cool_metal_logo_alpha)
)


def ruby_fu_cool_metal_logo(run_mode, text, size, font, bg_color, gradient)
  # img = Gimp::Image::new(256, 256, RGB)
  img = gimp_image_new(600, 256, RGB)
  begin
    puts "text = #{text}"
    text_layer = gimp_text_fontname(img, -1, 0, 0, text, 0, 1, size, PIXELS,
				    font)
    # img.undo_disable
    gimp_image_undo_disable(img)
    # text_layer.name = text
    puts "text = #{text}"
    gimp_drawable_set_name(text_layer, "Cool Metal")
    apply_cool_metal_logo_effect(img, text_layer, size, bg_color, gradient)
    # img.undo_enable
    gimp_image_undo_enable(img)
  rescue
    # img.delete
    gimp_image_delete(img)
    raise
  end
  # Gimp::Display::new(img)
  return gimp_display_new(img)
end

RubyFu.register(
  "ruby-fu-cool-metal-logo",
  "Metallic logos with reflections and perspective shadows",
  "Metallic logos with reflections and perspective shadows",
  "Spencer Kimball & Rob Malda & Masahiro Sakai",
  "Spencer Kimball & Rob Malda",
  "2001",
  "<Toolbox>/Xtns/Languages/Ruby-Fu/Cool Metal...",
  "",
  [
    ParamDef.STRING("text", "Text", 'Cool Metal'),
    ParamDef.INT32("font_size", "Font Size (pixels)", 100),
    ParamDef.STRING("font", "Font", "-*-Crillee-*-r-*-*-24-*-*-*-p-*-*-*"),
    ParamDef.COLOR("bg_color", "Background Color", Rgb.new(1.0, 1.0, 1.0)),
    ParamDef.STRING("gradient", "Gradient", "Horizon 1"),
  ],
  [
    ParamDef.IMAGE('image', 'Image'),
  ],
  &method(:ruby_fu_cool_metal_logo)
)

RubyFu.main
