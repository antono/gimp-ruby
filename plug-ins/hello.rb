#!/usr/bin/env ruby

require 'gimp'
include Gimp

#include PDB::Access

RubyFu.register(
  'plug-in-hello',
	'Hello, world!',
	'Displays "Hello, world!" in a dialog',
	'Scott Lembcke',
	'Copyright Scott Lembcke',
	'2006',
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Hello World!',
	'',
	[
	  ParamDef.INT32('run-mode', 'Run mode'),
	  	ParamDef.IMAGE('image', 'Input image'),
	  ParamDef.DRAWABLE('drawable', 'Input drawable'),
	],
	[ParamDef.STATUS('status', 'Status')]
) do|run_mode|  
	PDB.gimp_message "Hello, world!" unless run_mode == RUN_NONINTERACTIVE

	[]
end

RubyFu.main
