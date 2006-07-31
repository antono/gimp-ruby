require 'gimp'

RubyFu.register(
  'ruby-fu-console', #procedure name
  'Starts an irb session in a console.', #blurb
  nil, #help
  nil, #author
  nil, #copyright
  nil, #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/IRB Console', #menupath
  nil, #image types
  nil, #params
  nil #results
) do
  require 'irb'
  require 'gimp_oo'
  
  include Gimp
  include PDB::Access

  console = File.join(ENV['RUBYLIB'], 'ruby-fu-console')
  $stdin = $stdout = open("|#{console}", 'w+')
  ARGV.clear
  
  IRB.setup(nil)
  irb = IRB::Irb.new(nil, IRB::StdioInputMethod.new)
  IRB.conf[:MAIN_CONTEXT] = irb.context
  
  IRB.conf[:AUTO_INDENT] = true
  IRB.conf[:USE_READLINE] = false
  
  catch(:IRB_EXIT) do
    irb.eval_input
  end
  
  nil
end

RubyFu.main
