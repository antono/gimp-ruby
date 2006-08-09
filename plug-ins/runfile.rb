require 'rubyfu'

include Gimp
include RubyFu

module RubyFu
  class Procedure
    attr_reader :fullparams, :type
  end

  def self.test_proc(procname, drawable)
    proc = @@procedures[procname]
    
    params = proc.fullparams
    case proc.type
    when :toolbox:
      proc.run(Param.INT32(RUN_INTERACTIVE))
    when :image:
      image = Param.IMAGE(PDB.gimp_drawable_get_image(drawable))
      drawable = Param.DRAWABLE(drawable)
      proc.run(Param.INT32(RUN_INTERACTIVE), image, drawable)
    when :internal
      args = RubyFu.dialog("Testing #{procname}", procname, params)
      proc.run(*args)
    end
  end
end

register(
  'ruby-fu-runfile', #procedure name
  nil, #blurb
  nil, #help
  'Scott Lembcke', #author
  ' Scott Lembcke', #copyright
  '2006', #date
  '<Toolbox>/Xtns/Languages/Ruby-Fu/Run File', #menupath
  nil, #image types
  [
    ParamDef.FILE('file', 'File'),
    ParamDef.STRING('procedure', 'Procedure name', 'ruby-fu-'),
    ParamDef.DRAWABLE('drawable', 'Drawable (if needed)'),
  ], #params
  [] #results
) do|run_mode, filename, procname, drawable|
  Shelf['ruby-fu-last-runfile'] = [filename, procname, drawable]

  load(filename)
  RubyFu.test_proc(procname, drawable)
  
  nil
end
