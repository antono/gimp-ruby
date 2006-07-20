#!/usr/bin/env ruby

require 'gimp'

RubyFu.register(
  'ruby-fu-test1', #procedure name
	nil, #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	nil, #menupath
	nil, #image types
	nil, #params
	nil #results
) do
	#nothing
end

RubyFu.register(
  'ruby-fu-test2', #procedure name
	nil, #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	nil, #menupath
	nil, #image types
	[], #params
	[] #results
) do
	#nothing
end

RubyFu.register(
  'ruby-fu-test-echo', #procedure name
	nil, #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	nil, #menupath
	nil, #image types
	[
	  RubyFu::ParamDef.INT32('int32', 'int32', 1),
	  RubyFu::ParamDef.INT16('int16', 'int16', 1),
	  RubyFu::ParamDef.INT8('int8', 'int8', 1),
	  RubyFu::ParamDef.FLOAT('float', 'float', 1.1),
	  RubyFu::ParamDef.STRING('string', 'string', 'hello'),
	  RubyFu::ParamDef.INT32('length1', 'length1', 1),
	  RubyFu::ParamDef.INT32ARRAY('int32array', 'int32array', [1, 2, 3]),
	  RubyFu::ParamDef.INT32('length2', 'length2', 1),
	  RubyFu::ParamDef.INT16ARRAY('int16array', 'int16array', [1, 2, 3]),
	  RubyFu::ParamDef.INT32('length3', 'length3', 1),
	  RubyFu::ParamDef.INT8ARRAY('int8array', 'int8array', 'abc'),
	  RubyFu::ParamDef.INT32('length4', 'length4', 1),
	  RubyFu::ParamDef.FLOATARRAY('floatarray', 'floatarray', [1.1, 2.2, 3.3]),
	  RubyFu::ParamDef.INT32('length5', 'length5', 1),
	  RubyFu::ParamDef.STRINGARRAY('stringarray', 'stringarray', ['one', 'two', 'three']),
	  RubyFu::ParamDef.COLOR('color', 'color', Gimp::Rgb.new(1.0, 1.0, 1.0)),
	  RubyFu::ParamDef.PARASITE('parasite', 'parasite', Gimp::Parasite.new('name', 999, 'data')),
	], #params
	[
	  Gimp::ParamDef.INT32('int32', 'int32'),
	  Gimp::ParamDef.INT16('int16', 'int16'),
	  Gimp::ParamDef.INT8('int8', 'int8'),
	  Gimp::ParamDef.FLOAT('float', 'float'),
	  Gimp::ParamDef.STRING('string', 'string'),
	  Gimp::ParamDef.INT32('length1', 'length1'),
	  Gimp::ParamDef.INT32ARRAY('int32array', 'int32array'),
	  Gimp::ParamDef.INT32('length2', 'length2'),
	  Gimp::ParamDef.INT16ARRAY('int16array', 'int16array'),
	  Gimp::ParamDef.INT32('length3', 'length3'),
	  Gimp::ParamDef.INT8ARRAY('int8array', 'int8array'),
	  Gimp::ParamDef.INT32('length4', 'length4'),
	  Gimp::ParamDef.FLOATARRAY('floatarray', 'floatarray'),
	  Gimp::ParamDef.INT32('length5', 'length5'),
	  Gimp::ParamDef.STRINGARRAY('stringarray', 'stringarray'),
	  Gimp::ParamDef.COLOR('color', 'color'),
	  Gimp::ParamDef.PARASITE('parasite', 'parasite'),
	] #results
) do|int32, int16, int8, float, string,
     l1, int32array, l2, int16array, l3, int8array, l4, floatarray, l5, stringarray,
     color, parasite|
     
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

RubyFu.register(
  'ruby-fu-test-call-echo', #procedure name
	'Check that parameter conversion is safe.', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/Echo', #menupath
	nil, #image types
	[], #params
	[] #results
) do|run_mode|

	args = [
	  123,
	  -1,
	  129,
	  123.123,
	  'stringing it',
	  3,
	  [1, 2, 3],
	  3,
	  [1, 2, 3],
	  3,
	  'abc',
	  3,
	  [1.1, 2.2, 3.3],
	  3,
	  ['one', 'two', 'three'],
	  Gimp::Rgb.new(1.0, 1.0, 1.0),
	  Gimp::Parasite.new("a rose", 12, "some data")
	]
	
	values = PDB.ruby_fu_test_echo(*args)
	
  if args == values
    Gimp.message "Success!"
  else
    Gimp.message "Failure"
  end
		
	nil
end

RubyFu.register(
  'ruby-fu-test-shelf', #procedure name
	'Test that the shelf works correctly.', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/Shelf', #menupath
	nil, #image types
	[], #params
	[] #results
) do|run_mode|
  def test_method
    return false if Gimp::Shelf['badstring'] != nil
    
    Gimp.message "Previous value = #{Gimp::Shelf['goodstring'].inspect}"
    
    obj = ['wow', 1]
    Gimp::Shelf['goodstring'] = obj
    return false if Gimp::Shelf['goodstring'] != obj
    
    return true
  end
  
  if test_method
    Gimp.message "Success!"
  else
    Gimp.message "Failure"
  end

	nil
end

RubyFu.register(
  'ruby-fu-test-crash', #procedure name
	nil, #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	nil, #menupath
	nil, #image types
	[], #params
	[] #results
) do||
  raise "This is a test exception"
  
  nil
end

def test_exception(e)
   begin
    yield
  rescue e
    puts 'Success!'
    return
  end
  
  puts 'Failure'
  $failure = true
end

RubyFu.register(
  'ruby-fu-test-call', #procedure name
	'Check proper errors are raised when calling methods improperly.', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/Call', #menupath
	nil, #image types
	[], #params
	[] #results
) do|run_mode|
  require 'stringio'
  $stdout = StringIO.new
  
  print 'Calling with bad params ... '
  test_exception(TypeError){PDB.gimp_message(123)}
  
  print 'Calling with too many params ... '
  test_exception(ArgumentError){PDB.gimp_message(123, 123)}
  
  ##THIS CRASHES HORRIBLY. NOT SURE IF IT'S MY FAULT OR NOT.
  # printf 'Calling error ... '
  # test_exception(PDB::CallingError) do
  #   Gimp.run_procedure('ruby-fu-test-crash', [Gimp::Param.INT32(123)])
  # end

  printf 'Exucution error ... '
  test_exception(PDB::ExecutionError){PDB.ruby_fu_test_crash}
  
  Gimp.message('Tests ' + ($failure ? 'failed' : 'successful') + ":\n" + $stdout.string)
  $stdout = STDOUT
  
  nil
end

RubyFu.register(
  'ruby-fu-test-OO', #procedure name
	'Test that the object functionality works', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/OO', #menupath
	nil, #image types
	[], #params
	[Gimp::ParamDef.IMAGE('image', 'Image')] #results
) do|run_mode|
  PDB.verbose = true
  require 'gimp_oo'
  
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

RubyFu.register(
  'ruby-fu-test-dialog', #procedure name
	'runs dialog', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/dialog', #menupath
	nil, #image types
	[
    RubyFu::ParamDef.INT32('INT32', '', 32),
    RubyFu::ParamDef.TOGGLE('TOGGLE', '', 1),
    RubyFu::ParamDef.INT16('INT16', '', 16),
    RubyFu::ParamDef.INT8('INT8', '', 8),
    RubyFu::ParamDef.FLOAT('FLOAT', '', 123.4),
    RubyFu::ParamDef.SPINNER('SPINNER', '', 6, (1.5..9.1), 0.01),
    RubyFu::ParamDef.SLIDER('SLIDER', '', 6, (1.5..9.1), 0.01),
    RubyFu::ParamDef.STRING('STRING', '', 'string'),
    RubyFu::ParamDef.COLOR('COLOR', '', Gimp::Rgb.new(1.0, 0.0, 0.0, 0.0)),
  ], #params
	[] #results
) do|run_mode, *params|
  
  Gimp.message(params.join("\n"))

  nil
end

RubyFu.register(
  'ruby-fu-test-dialog2', #procedure name
	'runs dialog', #blurb
	nil, #help
	nil, #author
	nil, #copyright
	nil, #date
	'<Toolbox>/Xtns/Languages/Ruby-Fu/Test/more dialog', #menupath
	nil, #image types
	[
    RubyFu::ParamDef.IMAGE('IMAGE', ''),
    RubyFu::ParamDef.DRAWABLE('DRAWABLE', ''),
    RubyFu::ParamDef.CHANNEL('CHANNEL', ''),
    RubyFu::ParamDef.LAYER('LAYER', ''),
    RubyFu::ParamDef.FONT('FONT', '', 'Monaco'),
    RubyFu::ParamDef.FILE('FILE', '', '/usr/share/dict/words'),
    RubyFu::ParamDef.DIR('DIR', '', '/usr/share/dict/'),
    RubyFu::ParamDef.PALETTE('PALETTE', '', 'Lights'),
    RubyFu::ParamDef.GRADIENT('GRADIENT', '', 'CD'),
    RubyFu::ParamDef.PATTERN('PATTERN', '', 'Pine'),
    RubyFu::ParamDef.BRUSH('BRUSH', '', 'Circle (11)'),
  ], #params
	[] #results
) do|run_mode, *params|
  
  Gimp.message(params.join("\n"))

  nil
end

RubyFu.main