#!/usr/bin/env ruby

GC.disable

require 'gimp'
include Gimp

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
	  ParamDef.INT32('run-mode', 'Run mode'),
	],
	nil
) do|run_mode|  
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
