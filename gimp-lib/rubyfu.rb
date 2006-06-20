module RubyFu
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
      
    	PDB.gimp_plugin_menu_register(@name, @menubasepath) if @menubasepath
    end
    
    def run(*args)
      #TODO check args (exit with PDB_CALLING_ERROR)
      values = @function.call(*args)
      #TODO check return values and handle exceptions
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
    rescue
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