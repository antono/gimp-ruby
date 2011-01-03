# Gimp-Ruby Tutorial

## GIMP Scripting in a Nutshell

Just about everything that you can do in the GUI is possible to do using
GIMP’s PDB, or Procedural Database. The PDB is a collection of procedures
that can be called to control the GIMP. To see the entire list of procedures
available, go to the Xtns menu in the toolbox and select Procedure Browser.
You should be able to find many procedures that will be familiar.

For instance, the gimp-edit-bucket-fill procedure is what you would
call if you wanted to use the paint bucket tool from a script.

So to write a GIMP plug-in script, all you have to do is figure out how
you would perform an action using GIMP’s GUI and translate that into
a series of procedure calls.

## Creating New Procedures with Ruby-Fu:

Ruby-Fu is part of Gimp-Ruby, and allows you to easily create new PDB
procedures. Let’s look at the `helloworld.rb` tutorial script.

First of all, there are a lot of constants and methods used from both
the Gimp and RubyFu modules. Include the RubyFu module last so that you
don’t cover up some of the RubyFu classes such as ParamDef with their
counterparts in Gimp.

Creating a new procedure really only takes one method call. (Granted, it’s a very big one.)

    manifest = {
      :name      => 'ruby-fu-hello-world'
      :blurb     => 'Display a message.',
      :help      => 'Makes a new image to display a message',
      :author    => 'Scott Lembcke',
      :copyright => 'Scott Lembcke',
      :date      => '2006',
      :menulabel => 'Hello World',
      :params    => [
        ParamDef.STRING('message', 'Message', 'Hello World'),
        ParamDef.FLOAT('size', 'Font Size (Pixels)', 48),
        ParamDef.FONT('font', 'Font', 'verdana'),
        ParamDef.COLOR('color', 'Color', Color(0.0, 0.0, 0.0)),
      ],
      :results => [
        ParamDef.IMAGE('image', 'Image'),
      ]
    }
    RubyFu.register(manifest) do
      ...
    end

The `:name` value is the new procedure’s name. This is how it will be
accessible through the PDB. The standard is to `use-hyphen-seperated-words`
for your procedure’s name. The next few arguments should be pretty
straight forward. The menulabel is how the GIMP will refer to
your procedure from its GUI, including menu items and dialogs.

Next come the parameter and return value definitions. To avoid headaches,
GIMP procedures must define both their parameters and return values.
This is done by creating a list of RubyFu::ParamDef objects.

The last thing that the `RubyFu.register` method needs is a block.
This block is what is executed when your procedure is called.

For a more in depth reference, refer to the Gimp-Ruby Guide.

Now that we’ve defined a procedure let’s make it do something.

    do |run_mode, message, size, font, color|
      include PDB::Access
      ...
    end

First, note that the block takes an additional `run_mode` parameter.
This is because later we will be installing a menu item for our
procedure. The run mode parameter will tell you if your procedure
was called interactively (from the user through GIMP’s GUI),
non-interactively (from another procedure), or if it should be
run with the last values. This is primarily used by Ruby-Fu to decide
if it should display a dialog or not, but it is passed on in case
it’s useful to you.

The PDB::Access module allows you to use all of the PDB procedures
as methods without needing to call them as methods of the PDB module.
This is implemented using `#method_missing`, so any unknown method
will be assumed to be a PDB call. By including PDB::Access inside
the block, we ensure that we aren’t accidentally calling PDB
procedures outside of a running procedure.

Now, if we want our procedure to create a new image for a text
message, we probably want its size to reflect the size of the text.
Fortunately, there is a PDB procedure to do just that, 
gimp-text-get-extents. To call a PDB procedure, simply call it as a
method of the PDB or PDB::Access modules, changing the hyphens to
underscores so that it is a valid Ruby method name.

Most of the constants referred to in the PDB browser are defined on
the Gimp module. Additionally, true, false, and nil are automatically
converted into the integers 1, 0, and -1 for you.

    bounds = gimp_text_get_extents_fontname(message, size, PIXELS, font)
    width, height, ascent, descent = *bounds

Next we want to create a new image and layer to draw into. Ruby classes
have been provided to help you out. These are shallow wrappers around
the PDB procedures for handling GIMP types. By calling `Gimp::Image.new`
You’re actually calling the gimp-image-new procedure. Instance methods
also work pretty much like you’d expect.

    img = Image.new(width, height, RGB)
    layer = Layer.new(img, width, height, RGB_IMAGE, "hello", 100, NORMAL_MODE)
    layer.fill(WHITE_FILL)
    img.add_layer(layer, nil)

When changing GIMP settings like the foreground/background color, it’s not
nice to leave them that way when your procedure exits. An easy way to
handle that is to push the context using the gimp-context-push procedure.

In addition to classes for object types, the Gimp module also contains
modules that wrap PDB calls. This allows us to take things one step
further, the Gimp::Context.push method can accept a block. If you supply
one, it will call gimp-context-pop in an ensure block. That way even if
your code raises an exception, the user’s settings will be preserved.

    Context.push do
      Context.set_foreground(color)
      text_layer = gimp_text_fontname(img, layer, 0, 0, message, 0, true, size, PIXELS, font)
      gimp_floating_sel_anchor(text_layer)
    end

You’ll want to create a display so that the user can see the new image.
Lastly, you’ll want to return the image. GIMP doesn’t require this, but
it could be useful to other procedure developers.

    Display.new(img)
      img
    end

To register a menu for your plugin, use the `RubyFu.menu_register` method.
It simply takes the name of the procedure and the menu path where you want
to install the menu item. The name of the menu item is the string you
passed to `RubyFu.register` for the menulabel argument.

    menu_register('ruby-fu-hello-world', '<Toolbox>/Xtns/Languages/Ruby-Fu/Examples')

Depending on where you installed the procedure, you may need to accept
additional parameters to your procedure’s block. Check the Gimp-Ruby
Guide for more information.

Finally, putting the whole thing together:

    #!/usr/bin/env ruby
    
    require 'rubyfu'
    
    include Gimp
    include RubyFu
    
    manifest = {
      :name      => 'ruby-fu-hello-world'
      :blurb     => 'Display a message.',
      :help      => 'Makes a new image to display a message',
      :author    => 'Scott Lembcke',
      :copyright => 'Scott Lembcke',
      :date      => '2006',
      :menulabel => 'Hello World',
      :params    => [
        ParamDef.STRING('message', 'Message', 'Hello World'),
        ParamDef.FLOAT('size', 'Font Size (Pixels)', 48),
        ParamDef.FONT('font', 'Font', 'verdana'),
        ParamDef.COLOR('color', 'Color', Color(0.0, 0.0, 0.0)),
      ],
      :results => [
        ParamDef.IMAGE('image', 'Image'),
      ]
    }

    RubyFu.register(manifest) do |run_mode, message, size, font, color|
      include PDB::Access
    
      bounds = gimp_text_get_extents_fontname(message, size, PIXELS, font)
      width, height, ascent, descent = *bounds
    
      img = Image.new(width, height, RGB)
      layer = Layer.new(img, width, height, RGB_IMAGE, "hello", 100, NORMAL_MODE)
      layer.fill(WHITE_FILL)
      img.add_layer(layer, nil)
    
      Context.push do
        Context.set_foreground(color)
    
        text_layer = gimp_text_fontname(img, layer, 0, 0, message, 0, true, size, PIXELS, font)
        gimp_floating_sel_anchor(text_layer)
      end
    
      Gimp::Display.new(img)
    
      img
    end
    
    RubyFu.menu_register('ruby-fu-hello-world', '<Toolbox>/Xtns/Languages/Ruby-Fu/Examples')


## Installing Your Plugins

To install a new script, you can use the gimptool-2.0 utility.
(Make sure you are using the correct one for GIMP 2.4 or better if you
have multiple GIMP versions installed)

    gimptool-2.0 --install-bin helloworld.rb

That will install the plug-in locally for your user.

## Development Tools

### Irb Console

In the GIMP menus, go to Xtns/Ruby-Fu/Irb Console. It doesn’t yet
provide some of the features you may be used to such as history,
or auto complete, but it does allow you to use all of Irb’s base features.

Before starting the console, the Gimp, RubyFu, and PDB::Access modules
are included for you.

### Run/Rerun File

These allow you to run a Ruby-Fu script without requiring you to install it.
You can find them in the menus along with the Irb Console. Try it with the
helloworld.rb script. Select the it using the file browser, and enter
`ruby-fu-hello-world` as the procedure name. The drawable will be ignored
as the procedure is not image based.

Rerun File simply runs the last file run with Run File.
