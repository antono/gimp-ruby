#!/usr/bin/env ruby

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

require "rubyfu"

TestMenu = RubyFu.menu_branch_register(RubyFu::RubyFuMenu, "Test")

RubyFu.register(:name => "ruby-fu-test1") do
  #nothing
end

RubyFu.register(:name => "ruby-fu-test2") do
  #nothing
end

RubyFu.register(
  :name   => "ruby-fu-test-echo",
  :params => [
    RubyFu::ParamDef.INT32("int32", "int32", 1),
    RubyFu::ParamDef.INT16("int16", "int16", 1),
    RubyFu::ParamDef.INT8("int8", "int8", 1),
    RubyFu::ParamDef.FLOAT("float", "float", 1.1),
    RubyFu::ParamDef.STRING("string", "string", "hello"),
    RubyFu::ParamDef.INT32("length1", "length1", 1),
    RubyFu::ParamDef.INT32ARRAY("int32array", "int32array", [1, 2, 3]),
    RubyFu::ParamDef.INT32("length2", "length2", 1),
    RubyFu::ParamDef.INT16ARRAY("int16array", "int16array", [1, 2, 3]),
    RubyFu::ParamDef.INT32("length3", "length3", 1),
    RubyFu::ParamDef.INT8ARRAY("int8array", "int8array", "abc"),
    RubyFu::ParamDef.INT32("length4", "length4", 1),
    RubyFu::ParamDef.FLOATARRAY("floatarray", "floatarray", [1.1, 2.2, 3.3]),
    RubyFu::ParamDef.INT32("length5", "length5", 1),
    RubyFu::ParamDef.STRINGARRAY("stringarray", "stringarray", ["one", "two", "three"]),
    RubyFu::ParamDef.COLOR("color", "color", Gimp::Rgb.new(1.0, 1.0, 1.0)),
    RubyFu::ParamDef.PARASITE("parasite", "parasite", Gimp::Parasite.new("name", 999, "data")),
  ],
  :results => [
    Gimp::ParamDef.INT32("int32", "int32"),
    Gimp::ParamDef.INT16("int16", "int16"),
    Gimp::ParamDef.INT8("int8", "int8"),
    Gimp::ParamDef.FLOAT("float", "float"),
    Gimp::ParamDef.STRING("string", "string"),
    Gimp::ParamDef.INT32("length1", "length1"),
    Gimp::ParamDef.INT32ARRAY("int32array", "int32array"),
    Gimp::ParamDef.INT32("length2", "length2"),
    Gimp::ParamDef.INT16ARRAY("int16array", "int16array"),
    Gimp::ParamDef.INT32("length3", "length3"),
    Gimp::ParamDef.INT8ARRAY("int8array", "int8array"),
    Gimp::ParamDef.INT32("length4", "length4"),
    Gimp::ParamDef.FLOATARRAY("floatarray", "floatarray"),
    Gimp::ParamDef.INT32("length5", "length5"),
    Gimp::ParamDef.STRINGARRAY("stringarray", "stringarray"),
    Gimp::ParamDef.COLOR("color", "color"),
    Gimp::ParamDef.PARASITE("parasite", "parasite"),
  ]
) do |int32, int16, int8, float, string, l1, int32array, l2, int16array, l3, int8array, l4, floatarray, l5, stringarray, color, parasite|
     
  puts parasite
  
  [int32, int16, int8, float, string,
    l1, int32array,
    l2, int16array,
    l3, int8array,
    l4, floatarray,
    l5, stringarray, 
    color,
    parasite
  ]
end

RubyFu.register(:name => "ruby-fu-test-call-echo",
  :blurb => "Check that parameter conversion is safe.",
  :menupath => "Echo") do |run_mode|

  args = [
    123,
    -1,
    129,
    123.123,
    "stringing it",
    3,
    [1, 2, 3],
    3,
    [1, 2, 3],
    3,
    "abc",
    3,
    [1.1, 2.2, 3.3],
    3,
    ["one", "two", "three"],
    Gimp::Rgb.new(1.0, 1.0, 1.0),
    Gimp::Parasite.new("a rose", 12, "some data")
  ]
  
  values = PDB.ruby_fu_test_echo(*args)
	
  if args == values
    Gimp.message "Success!"
  else
    Gimp.message "Failure"
  end
end


RubyFu.menu_register("ruby-fu-test-call-echo", TestMenu)


RubyFu.register(
  :name  => "ruby-fu-test-shelf",
  :blurb => "Test that the shelf works correctly.",
  :menupath => "Shelf") do |run_mode|

  def test_method
    return false if Gimp::Shelf["badstring"] != nil
    
    Gimp.message "Previous value = #{Gimp::Shelf['goodstring'].inspect}"
    
    obj = ["wow", 1]
    Gimp::Shelf["goodstring"] = obj
    return false if Gimp::Shelf["goodstring"] != obj
    
    return true
  end
  
  if test_method
    Gimp.message "Success!"
  else
    Gimp.message "Failure"
  end
end

RubyFu.menu_register("ruby-fu-test-shelf", TestMenu)


RubyFu.register(:name => "ruby-fu-test-crash") do
  raise "This is a test exception"
end

def test_exception(e)
  begin
    yield
  rescue e
    puts "Success!"
    return
  end

  puts "Failure"
  $failure = true
end

RubyFu.register(
  :name  => "ruby-fu-test-call",
  :blurb => "Check proper errors are raised when calling methods improperly.",
  :menupath => "Call") do |run_mode|

  require "stringio"
  $stdout = StringIO.new
  
  print "Calling with bad params ... "
  test_exception(TypeError){PDB.gimp_message(123)}
  
  print "Calling with too many params ... "
  test_exception(ArgumentError){PDB.gimp_message(123, 123)}
  
  #THIS CRASHES HORRIBLY. NOT SURE IF IT'S MY FAULT OR NOT.
  #printf 'Calling error ... '
  #test_exception(PDB::CallingError) do
  #  #Gimp.run_procedure('ruby-fu-test-crash', [Gimp::Param.INT32(123)])
  #  PDB.plug_in_script_fu_eval(Gimp::Interactive, '(ruby-fu-test-crash 123)')
  #end

  printf "Execution error ... "
  test_exception(PDB::ExecutionError){PDB.ruby_fu_test_crash}
  
  Gimp.message("Tests " + ($failure ? "failed" : "successful") + ":\n" + $stdout.string)
  $stdout = STDOUT
end

# FIXME: RubyFu.menu_register("ruby-fu-test-call", TestMenu)

RubyFu.register(
  :name => "ruby-fu-test-OO",
  :blurb => "Test that the object functionality works",
  :menuath => "OO",
  :results => [
    Gimp::ParamDef.IMAGE("image", "Image")
  ]) do |run_mode|

  PDB.verbose = true
  
  #force all classes and modules to load
  Gimp::Drawable
  Gimp::Brush
  Gimp::Channel
  Gimp::Display
  Gimp::Gradient
  Gimp::Image
  Gimp::Layer
  Gimp::Palette
  Gimp::Vectors

  Gimp::Context
  Gimp::Edit 
  Gimp::Progress
  Gimp::Selection
  
  #constructor call
  img = Gimp::Image.new(100, 100, Gimp::RGB)
  
  #module call with block pair
  Gimp::Context.push do
    #instance call with block pair
    img.undo_disable {}
    Gimp::Display.new(img)
  end
  
  Gimp.message "Successful!"
  
  img
end

RubyFu.menu_register("ruby-fu-test-OO", TestMenu)


RubyFu.register(
  :name => "ruby-fu-test-dialog",
  :blurb => "runs dialog",
  :menupath => "Dialog",
  :params => [
    RubyFu::ParamDef.INT32("int32", "INT32", 32),
    RubyFu::ParamDef.TOGGLE("toggle", "TOGGLE", 1),
    RubyFu::ParamDef.INT16("int16", "INT16", 16),
    RubyFu::ParamDef.INT8("int8", "INT8", 8),
    RubyFu::ParamDef.FLOAT("float", "FLOAT", 123.4),
    RubyFu::ParamDef.SPINNER("spinner", "SPINNER", 6, (1.5..9.1), 0.01),
    RubyFu::ParamDef.SLIDER("slider", "SLIDER", 6, (1.5..9.1), 0.01),
    RubyFu::ParamDef.STRING("string", "STRING", "string"),
    RubyFu::ParamDef.TEXT("text", "TEXT", "multi\nline\nstring"),
    RubyFu::ParamDef.COLOR("color", "COLOR", Gimp::Rgb.new(1.0, 0.0, 0.0, 0.0)),
    RubyFu::ParamDef.ENUM("enum", "ENUM", 0, "GimpBlendMode")
  ]) do |run_mode, *params|
  Gimp.message(params.join("\n"))
end

RubyFu.menu_register("ruby-fu-test-dialog", TestMenu)


RubyFu.register(
  :name => "ruby-fu-test-dialog2",
  :blurb => "runs dialog",
  :menupath => "More dialog",
  :params => [
    RubyFu::ParamDef.LIST("list", "LIST", ["a", "two", "3", "IV"]),
    RubyFu::ParamDef.IMAGE("image", "IMAGE"),
    RubyFu::ParamDef.DRAWABLE("drawable", "DRAWABLE"),
    RubyFu::ParamDef.CHANNEL("channel", "CHANNEL"),
    RubyFu::ParamDef.LAYER("layer", "LAYER"),
    RubyFu::ParamDef.FONT("font", "FONT", "Monaco"),
    RubyFu::ParamDef.FILE("file", "FILE", "/usr/share/dict/words"),
    RubyFu::ParamDef.DIR("dir", "DIR", "/usr/share/dict/"),
    RubyFu::ParamDef.PALETTE("palette","PALETTE", "Lights"),
    RubyFu::ParamDef.GRADIENT("gradient", "GRADIENT", "CD"),
    RubyFu::ParamDef.PATTERN("pattern", "PATTERN", "Pine"),
    RubyFu::ParamDef.BRUSH("brush", "BRUSH", "Circle (11)"),
  ]) do |run_mode, *params|
  Gimp.message(params.join("\n"))
end

RubyFu.menu_register("ruby-fu-test-dialog2", TestMenu)
