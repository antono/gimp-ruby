require 'rubyfu'

include Gimp
include RubyFu

register(
  'ruby-fu-rerunfile', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Rerun File', #menupath
  nil, #image types
  [], #params
  [] #results
) do|run_mode, filename, procname|
  last = Shelf['ruby-fu-last-runfile']
  
  if last
    PDB.ruby_fu_runfile(RUN_NONINTERACTIVE, *last)
  else
    Gimp.message 'No previous file to run'
  end
  
  nil
end
