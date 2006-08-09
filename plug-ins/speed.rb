tim = Time.now

require 'rubyfu'

include Gimp
include RubyFu

include PDB::Access

register(
  'ruby-fu-speed', #procedure name
  nil, #blurb
  nil, #help
  nil, #author
  nil, #copyright
  nil, #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Test/Speed', #menupath
  nil,
  nil, #params
  nil #results
) do
  Gimp.message "Plugin startup time is #{Time.now - tim} seconds."
  
  nil
end
