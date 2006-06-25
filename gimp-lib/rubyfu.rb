module RubyFu
  class CallError < Exception
  end
  
  class ResultError < Exception
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
      else
        @menubasepath = File.dirname menupath
        @menulabel = File.basename menupath
      end
      
      @imagetypes, *rem = *rem
      @params, @results, *rem = *rem
      
      @function = func
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
        @params,
        @results
      )
      
    	if @menubasepath
    	  PDB.gimp_plugin_menu_register(@name, @menubasepath)
    	end
    end
    
    def run(*args)
     params = @params ? @params : []
     results = @results ? @results : []
     
     if params.length > 0 and params[0].name == 'run-mode'
        runMode = args[0].data
      else
        runMode = Gimp::RUN_NONINTERACTIVE
      end

      args = args.zip(params).collect do|arg, param|
        raise(CallError, "Bad argument") unless arg.type == param.type
        next arg.data
      end
     
      case runMode
      when Gimp::RUN_INTERACTIVE
        #use default values for now
        #TODO (much later) create an input dialog.
        warn "Interactive mode not implemented, sending default params."
        values = @function.call(*args)
      when Gimp::RUN_NONINTERACTIVE
        nargs = args.length
        nparams = params.length
        unless nargs == nparams
          raise(CallError, "Wrong number of arguments. (#{nargs} for #{nparams})")
        end
                
        values = @function.call(*args)
      when Gimp::RUN_WITH_LAST_VALS
        #use default values for now
        #TODO actually remember the last values
        warn "Last value mode not implemented, sending default params."
        values = @function.call(*args)
      end
      
      if values == nil
        values = []
      else
        *values = *values
      end

      #TODO check & convert return values and handle exceptions
      nvalues = values.length
      nresults = results.length
      unless nvalues == nresults
        #FIXME A more descriptive exception type would be good here.
        message = "Wrong number of return values. (#{nvalues} for #{nresults})"
        raise(ResultError, message)
      end
      
      begin
        values = values.zip(results).collect do|value, result|
          result.check(value)
          Gimp::Param.new(result.type, value)
        end
      rescue
        #FIXME more informative handling would be good.
        raise "return value type check failed?"
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
      params,
      results,
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
      values.unshift Param.STATUS(PDB_SUCCESS)
      
      return values
    rescue CallError
      PDB.gimp_message("A calling error has occured: #$!.message")
      return [Param.STATUS(PDB_CALLING_ERROR)]
    rescue Exception
      PDB.gimp_message "A #{$!.class} has occured: #{$!.message}\n#{$@.join("\n")}"
      return [Param.STATUS(PDB_EXECUTION_ERROR)]
    end
  end
  
  PLUG_IN_INFO = Gimp::PlugInInfo.new(
  	nil,
  	nil,
  	method(:query),
  	method(:run)
  )
  
  def self.main
    Gimp.main(PLUG_IN_INFO)
  end
end