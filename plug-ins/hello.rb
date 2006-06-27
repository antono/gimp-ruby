#!/usr/bin/env ruby

require 'gimp'
#include RubyFu

#include PDB::Access

RubyFu.register(
  'ruby-fu-hello',
	'Hello, world!',
	'Displays "Hello, world!" in a dialog',
	'Scott Lembcke',
	'Copyright Scott Lembcke',
	'2006',
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Hello World!',
	'',
	[
	  RubyFu::ParamDef.STRING('stuff', 'test', 'test string'),
	],
	nil
) do|run_mode, stuff|
  run_mode = Gimp::EnumNames::RunMode[run_mode]
  Gimp.message "Run mode: #{run_mode}\nStuff: #{stuff.inspect}"
	PDB.ruby_fu_hello2
	nil
end

RubyFu.register(
  'ruby-fu-hello2',
	'Hello, world!',
	'Displays "Hello, world!" in a dialog',
	'Scott Lembcke',
	'Copyright Scott Lembcke',
	'2006',
	nil,
	'',
	nil,
	nil
) do  
	Gimp.message "Hello, world 2!"
	nil
end

RubyFu.main
