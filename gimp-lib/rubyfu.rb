module RubyFu
  class CallError < Exception
  end
  
  class ResultError < Exception
  end
  
  class ParamDef < Gimp::ParamDef
    attr_reader :default, :subtype
    
    def self.method_missing(sym, *args)
      nargs = args.length
      if nargs == 3
        default = args.pop
        pdef = super(sym, *args)
        pdef.check(default)
        pdef.instance_variable_set(:@default, default)
        
        return pdef
      else
        return super
      end
    end
    
    def self.FONT(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :font)
      return value
    end
    
    def self.FILE(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :file)
      return value
    end
    
    def self.DIR(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :dir)
      return value
    end
    
    def self.PALETTE(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :palette)
      return value
    end
    
    def self.GRADIENT(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :gradient)
      return value
    end
    
    def self.PATTERN(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :pattern)
      return value
    end
    
    def self.BRUSH(*args)
      value = STRING(*args)
      value.instance_variable_set(:@subtype, :brush)
      return value
    end
    
    def self.TOGGLE(*args)
      value = INT32(*args)
      value.instance_variable_set(:@subtype, :toggle)
      return value
    end
    
    def self.SPINNER(name, desc, default, range, step)
      value = FLOAT(name, desc, default)
      value.instance_variable_set(:@subtype, :spinner)
      value.instance_variable_set(:@range, range)
      value.instance_variable_set(:@step, step)
      return value
    end      

    def self.SLIDER(name, desc, default, range, step)
      value = FLOAT(name, desc, default)
      value.instance_variable_set(:@subtype, :slider)
      value.instance_variable_set(:@range, range)
      value.instance_variable_set(:@step, step)
      return value
    end      
  end

  class Procedure
    def initialize(*args, &func)
      @name, *rem = *args
      @blurb, @help, *rem = *rem
      @author, @copyright, *rem = *rem
      @date, *rem = *rem
      
      menupath,*rem = *rem
      if menupath.empty?
        @menulabel = ""
        @type = :internal
      else
        @type = case menupath
        when /<Image>/: :image
        when /<Toolbox>/: :toolbox
        end
        
        @menubasepath = File.dirname menupath
        @menulabel = File.basename menupath
      end
      
      @imagetypes, *rem = *rem
      @params, @results, *rem = *rem
      
      @fullparams = preparams + @params
      
      @function = func
    end
    
    def preparams
      case @type
      when :toolbox
        [Gimp::ParamDef.INT32('run-mode', 'Run mode')]
      when :images
        [
          Gimp::ParamDef.INT32('run-mode', 'Run mode'),
          Gimp::ParamDef.IMAGE('image', 'Input image'),
          Gimp::ParamDef.DRAWABLE('drawable', 'Input drawable'),
        ]
      else
        []
      end
    end
        
    def query      
      Gimp.install_procedure(
        @name,
        @blurb,
        @help,
        @author,
        @copyright,
        @date,
        @menulabel,
        @imagetypes,
        Gimp::PLUGIN,
        (@fullparams.empty? ? nil : @fullparams),
        (@results.empty? ? nil : @results)
      )
      
    	if @menubasepath
    	  PDB.gimp_plugin_menu_register(@name, @menubasepath)
    	end
    end
    
    def default_args
      defArgs = @params.collect do|pdef|
        pdef.default if pdef.respond_to? :default
      end
    end
    
    def run_interactive(args)
      args += RubyFu.dialog(@name, @params)
      
      values = @function.call(*args)
    end
    
    def run_noninteractive(args, params)
      nargs = args.length
      nparams = params.length
      unless nargs == nparams
        raise(CallError, "Wrong number of arguments. (#{nargs} for #{nparams})")
      end
              
      values = @function.call(*args)
    end
    
    def run_last_vals(args)
      #use default values for now
      #TODO actually remember the last values
      warn "Last value mode not implemented, sending default params."
      args += default_args
      
      values = @function.call(*args)
    end
    
    def run(*args)
      #FIXME this could be improved
      if @fullparams.length > 0 and @fullparams[0].name == 'run-mode'
        runMode = args[0].data
      else
        runMode = Gimp::RUN_NONINTERACTIVE
      end
      
      args = args.zip(@fullparams).collect do|arg, param|
        raise(CallError, "Bad argument") unless arg.type == param.type
        next arg.data
      end
     
      values = case runMode
      when Gimp::RUN_INTERACTIVE:    run_interactive(args)
      when Gimp::RUN_NONINTERACTIVE: run_noninteractive(args, @fullparams)
      when Gimp::RUN_WITH_LAST_VALS: run_last_vals(args)
      end
      
      if values == nil
        values = []
      else
        *values = *values
      end

      nvalues = values.length
      nresults = @results.length
      unless nvalues == nresults
        message = "Wrong number of return values. (#{nvalues} for #{nresults})"
        raise(ResultError, message)
      end
      
      begin
        values = values.zip(@results).collect do|value, result|
          result.check(value)
          Gimp::Param.new(result.type, value)
        end
      rescue TypeError
        message = "Procedure return value type check failed: #{$!.message}"
        raise(TypeError, message)
      end
      
      return values
    end
  end
  
  @@procedures = {}
  
  def self.register(
    name,
    blurb,
    help,
    author,
    copyright,
    date,
    menupath,
    imagetypes,
    params,
    results,
    &function
  )
    proc = Procedure.new(
      String(name),
      String(blurb),
      String(help),
      String(author),
      String(copyright),
      String(date),
      String(menupath),
      String(imagetypes),
      Array(params),
      Array(results),
      &function
    )
    
    @@procedures[name] = proc
  end
  
  def self.query
    @@procedures.each_value{|proc| proc.query}
  end
  
  def self.run(name, *args)
    begin
      proc = @@procedures[name]
      
      values = proc.run(*args)
      values.unshift Gimp::Param.STATUS(Gimp::PDB_SUCCESS)
      
      return values
    rescue CallError
      PDB.gimp_message("A calling error has occured: #$!.message")
      return [Gimp::Param.STATUS(PDB_CALLING_ERROR)]
    rescue Exception
      PDB.gimp_message "A #{$!.class} has occured: #{$!.message}\n#{$@.join("\n")}"
      return [Gimp::Param.STATUS(Gimp::PDB_EXECUTION_ERROR)]
    end
  end
  
  PLUG_IN_INFO = Gimp::PlugInInfo.new(
  	nil,
  	nil,
  	method(:query),
  	method(:run)
  )
  
  def main
    Gimp.main(PLUG_IN_INFO)
  end
  module_function :main
end