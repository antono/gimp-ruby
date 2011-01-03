# Gimp-Ruby Guide

See also the Gimp-Ruby Tutorial

## Requirements

Gimp-Ruby requires the GIMP v2.4 or better, and Ruby v1.8.

The latest Gimp-Ruby is available from the GNOME Git.

## The Gimp module

The Gimp module is a low level wrapper of the GIMP’s C API.

## Gimp Constants

If you are writing a plug-in using Ruby-Fu, it’s unlikely that
the methods provided by Gimp will be useful, but the constants
are used, well, constantly.

    image = Gimp::Image.new(width, height, Gimp::RGB)
    layer.fill(GIMP::WHITE_FILL)

Not all enumerations listed for procedures in the PDB are standard
GIMP enumerations however. If you need to use one of these nonstandard
enumerations, it’s a good idea to define the values that you use in
your own script to enhance readability.

## The Gimp::EnumNames module

In addition, the Gimp module provides a set of hashes for the enumeration
value names. In addition to retrieving names, you can use them to
check if a value is a valid enumeration.

    Gimp::EnumNames::FillType[2] => "WHITE_FILL" 
    value = 5
    Gimp::EnumNames::FillType.has_key? value => false

## Object types

The Gimp module also contains a number of useful classes that wrap the GIMP types. These include:

- Image
- Drawable
- Layer (subclass of Drawable)
- Channel (subclass of Drawable)
- Brush
- Display
- Gradient
- Palette
- Vectors

These are simple wrappers of the procedures in the PDB. For instance, the
`Gimp::Image` class contains methods for all of the PDB procedures that
start with ‘gimp-image-’. Class methods are also provided when appropriate.

Internally, GIMP tracks these values using either integer ids or strings
depending on the type. To create an object based on an existing GIMP value,
use the #create method of the class.

    gradient = Gimp::Gradient.new('Deep Sea') #calls gimp-gradient-new
    gradient = Gimp::Gradient.create('Deep Sea')

The first line will actually call the gimp-gradient-new procedure, not what
you wanted in this case. The second line will create a Ruby object that refers
to the original gradient.

## PDB wrapper modules

Several modules are also provided for convenience.

- Context
- Edit
- Progress
- Selection

These are implemented similar to the classes. So the `Gimp::Edit` module
contains methods for all the PDB procedures beginning with ‘gimp-edit-’.

## Paired procedures

The PDB provides several procedures that are intended to used in pairs.
For instance, the ‘gimp-context-push’ and ‘gimp-context-pop’ procedures
are intended to be used together.

The Gimp-Ruby wrapper methods can help you out with this. For instance
`Gimp::Context.push`, if given a block, will ensure a call to `Gimp::Context.pop`
after the block is executed. That way even if your plugin causes an
exception after changing context values, the users context won’t be affected.

Methods that support such pairing are:

- `Gimp::Context.push/pop`
- `Gimp::Image#undo_group_start/undo_group_end`
- `Gimp::Image#undo_disable/undo_enable`

## The `Gimp::Shelf` module

The `Shelf` is a persistent way to store data between plugin activations.
It implements a hash-like interface by implementing the `.[]` and `.[]=` methods.
While the key must be a string, the value can be any object. Shelf values are
not saved between GIMP sessions.

## The PDB module

Gimp-Ruby provides access to the PDB through the PDB module.
Procedures can be called as methods of the PDB module itself.

    PDB.gimp_edit_bucket_fill(a, b, c, d, e)

### The PDB::Procedure class

`PDB::Procedure` objects are created using the `PDB.[]` method.
`PDB::Procedure` objects support the `#to_proc` method, so they
can be used in the same way as Proc objects.

    proc = PDB['gimp-edit-bucket-fill']
    some_method(&proc) #can be attached to method calls like ordinary Proc objects.
    proc.call(a, b, c, d, e) #calling a PDB::Procedure object

### The PDB::Access module

Including the PDB module directly isn’t a good idea. If you want to
include the PDB procedures into the namespace, include PDB::Access instead.

    include PDB # Not such a great idea
    include PDB::Access # Do this instead.

### Calling interactive procedures

Interactive procedures take a run-mode parameter as the first argument.
It’s pretty rare that you’d want to call a procedure interactively so the
PDB module passes `RUN_NONINTERACTIVE` for you. For example, the plug-in-gauss
requires a `run_mode parameter`, but you don’t need to to specify it.

    PDB.plug_in_gauss(image, drawable, 3.0, 3.0, Gimp::IIR)

If you actually wish to run a procedure interactively so it shows it’s GUI,
you can use the `PDB.call_interactive` method which takes the name of a
procedure and optionally an image and drawable if the procedure requires them.

    PDB.call_interactive('plug-in-gauss', image, drawable)

### true, false, and nil

The integers 1, 0 and -1 are often used as true, false, and nil values.
The PDB module supports this convention by converting true, false and nil
arguments to integers before invoking the procedure.

IMPORTANT!: PDB procedures cannot be called outside of a running procedure.
Gimp-Ruby doesn’t handle this gracefully yet. Doing so will likely end in a glib error.

## The RubyFu module

The RubyFu module provides a simple interface for defining new PDB procedures
in a manner similar to Script-Fu. It auto-magically provides many of the behaviors
that a good plugin should. For instance it handles returning the execution status
for the procedure call. It also provides a dialog for your procedures if they
are called interactively.

### The `RubyFu.register` method:

To register a block as a new PDB procedure, you use the `RubyFu.register` method.
To satiate the PDB’s thirst for information, it takes a hash and a block as arguments.

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
      # block
    end

- `:name` is the name used to call the procedure in the PDB.
  It must be unique from any other PDB procedure. Procedure names
  should have hyphen seperated words in their names.
- `:blurb` should be a short description of the procedure’s function.
  This will appear as a tooltip if the procedure is installed in a menu.
- `:help` should be more thorough than blurb. This will appear in the
  procedure browser to help other plugin developers understand how to
  use your procedure.
- `:author`, `:copyright`, and `:date` should be fairly straightforward.
- `:menulabel` is the name that will show up in the GUI for your procedure.
  This includes the menulabel for your procedure if you choose to install
  it as well as error/message dialogs. Supplying nil or omiting `:menulabel`
  will use `:name` instead.
- `:image_types` should be a comma separated string of supported image
  types. (RGB, INDEXED, or GRAY, with or without alpha) An "" can also be used.
  For instance, “RGB, GRAY” would mean RGB, RGBA, or GRAY image types are accepted.
- `:params` and `:results` should be arrays of `RubyFu::ParamDef` objects.

Except for `:name`, all parameters are optional and can be omited.

### Parameter and return value definitions

The `RubyFu::ParamDef` class is a subclass of `Gimp::ParamDef` and adds some
useful parameter subtypes and functionality. The general form for
creating a new `RubyFu::ParamDef` object is:

    RubyFu::ParamDef.TYPE('name', 'description', default_value)

description should be kept fairly short as it’s used as the label in the dialog.
The base GIMP parameter types are:

    GIMP Type	Ruby Type
    INT32	Integer
    INT16	Integer
    INT8	Integer (unsigned)
    FLOAT	Float
    STRING	String
    INT32ARRAY	Array
    INT16ARRAY	Array
    INT8ARRAY	String* (see note)
    FLOATARRAY	Array
    STRINGARRAY	Array
    COLOR	Gimp::Rgb
    DISPLAY	Gimp::Display
    IMAGE	Gimp::Image
    LAYER	Gimp::Layer
    CHANNEL	Gimp::Channel
    DRAWABLE	Gimp::Drawable
    SELECTION	Integer
    BOUNDARY	Integer
    VECTORS	Gimp::Vectors
    PARASITE	Gimp::Parasite
    STATUS	Integer

The `INT8ARRAY` type is implemented using Ruby strings. This is
because `INT8ARRAY` is used for data pointers more often than
arrays. Strings have limited char array capabilities if you
really need to use one as an array. This should be pretty rare though.

The array types, DISPLAY, SELECTION, BOUNDARY, VECTORS, PARASITE,
and STATUS types do not have an associated widget. Using them as
parameters in an interactive procedure is an error.

In addition to the base GIMP types, Ruby-Fu defines several subtypes.
These only show up in the PDB as the GIMP base types, but they do
affect the associated widget in the dialog created for your procedure
if it’s ran interactively. The following table lists what base types
the Ruby-Fu subtypes map onto, and what kind of widget they have
in the GUI.

    Ruby-Fu Type   Base GIMP Type	Dialog Widget
    FONT	   STRING	        Font selection
    FILE	   STRING	        File selection
    DIR		   STRING	        Directory selection
    PALETTE	   STRING	        Palette selection
    GRADIENT	   STRING	        Gradient selection
    PATTERN	   STRING	        Pattern selection
    BRUSH	   STRING	        Brush selection
    TOGGLE	   INT32	        Check button
    SPINNER	   FLOAT	        Spinner button
    SLIDER	   FLOAT	        Slider
    ENUM	   INT32	        combo box
    LIST	   STRING	        Combo box
    TEXT	   STRING	        Multiline text entry

Some of the Ruby-Fu subtypes have unique parameter initializers:

    ParamDef.SPINNER(name, desc, default, range, step_size)

Where `range` is a Ruby Range object, and `step_size` is the amount of
change when clicking on the slider bar.

    ParamDef.SLIDER(...) # Same as SPINNER.


    ParamDef.ENUM(name, desc, default, enum_name)

`enum_name` should be a string containing valid GIMP enum name
such as GimpFillType.

    ParamDef.LIST(name, desc, list)

Takes an Array of Strings instead of the default value.
The first string is used as the default.

### Return values

Your procedure returns the values returned by it’s block.
Normal Ruby multiple return value rules apply, so if you need to
return multiple values, simply return an array of values.

Notes:
Return values are checked for consistency with the definitions.
Returning the wrong number of values, or the wrong type will
result in your procedure exiting with an execution error.
Variable return values are not allowed.
Procedures that aren’t defined to return any values ignore values
returned from their associated block.

### The `RubyFu.menu_register` method

To create a menu item for your procedure you use the `RubyFu.menu_register` method.

    RubyFu.menu_register(procedure_name, menupath)

`procedure_name` should be the same as the `:name` you used in `RubyFu.register`.
`menu_path` should be of the form "/Path/To/My/Menu" where Domain is either
"Image" or "Toolbox".

By registering a menu for your procedure, you promote it to an interactive
procedure. Depending on the menu domain that you installed the procedure
into, there are certain parameters that your procedure is expected to take.
Procedures are expected to take a `run-mode` parameter when installed under,
and when installed under they are expected to take a 'run-mode', 'image',
and 'drawable' parameter. `RubyFu` adds the correct ParamDef’s for you, but
you still have to add the extra parameters to your block.

### Sample RubyFu.register call:

A complete procedure register example taken from the tutorial plugin:


    include RubyFu

    register(
      :name      => 'ruby-fu-hello-world',
      :blurb     => 'Display a message.',
      :help      => 'Makes a new image to display a message',
      :author    => 'Scott Lembcke',
      :copyright => 'Scott Lembcke',
      :date      => '2006',
      :menulabel => 'Hello World', #menulabel
      :params    => [
        ParamDef.STRING('message', 'Message', 'Hello World'),
        ParamDef.FLOAT('size', 'Font Size (Pixels)', 48),
        ParamDef.FONT('font', 'Font', 'verdana'),
        ParamDef.COLOR('color', 'Color', Color(0.0, 0.0, 0.0)),
      ],
      :results   => [
        ParamDef.IMAGE('image', 'Image'),
      ]
    ) do |run_mode, message, size, font, color|
      #procedure code goes here
    end

    menu_register('ruby-fu-hello-world', '<Toolbox>/Xtns/Languages/Ruby-Fu/Examples')

Notes:

The plugin isn’t an image based plugin, so we can omit `image_types` parameter.
Because a menu item was installed for the procedure, it is promoted to an
interactive procedure and the block must take an additional parameter for
the run mode.  While there isn’t actually a Xtns/Languages submenu, that’s
the real path for the Ruby-Fu submenu. I guess the GIMP people decided there
should be a way to group the language bindings together.

Tips:

Unless you like typing ‘Gimp’, and ‘RubyFu’ a lot, you’ll probably want to
include both modules into the namespace. If you do, include Gimp before
RubyFu because RubyFu subclasses several of the classes under Gimp, such as ParamDef.

Gimp-Ruby supports localization using the gettext library. (The wierd `_()` and
`N_(`) methods used in the gimp-ruby distribution plugins are used for gettext)
While there is no easy way that I know of yet to install localized plug-ins outside
of the Gimp-Ruby package, if you even want your plugin to be localizable, don’t
use single quotes as the gettext parser looks for double quoted strings.

You can call `RubyFu.register` multiple times in the same script to register several
procedures if you wish.  Ruby-Fu works by registering an END block that activates
the plug-in’s connection to GIMP after your plug-in script has been executed.
Then GIMP tells Ruby-Fu to run a particular procedure, if called interactively
the dialog is displayed at this point. To speed up your plug-in’s startup speed,
don’t require any unnecessary files outside of your procedures’ blocks if possible.
If it takes a minute to require your script, then it will take a minute for any
of your plug-in’s dialogs to appear.

## Contact:

- Scott Lembcke (original author) available for contact by email at slembcke(at)cvs.gnome.org.
- Antono Vasiljev (current maintainer) antono.vasiljev@gmail.com

Questions, comments, bug reports, and suggestions are all welcome.
