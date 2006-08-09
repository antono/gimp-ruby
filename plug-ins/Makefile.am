## Process this file with automake to produce Makefile.in

pluginexecdir = $(GIMP_PLUGINDIR)

scripts =  \
    coolmetal-logo.rb \
    sphere.rb         \
    irbconsole.rb     \
    runfile.rb        \
    rerunfile.rb      \
    gloom.rb          \
    sunset.rb         \
    helloworld.rb
    

test_scripts = \
    test.rb    \
    speed.rb

 
if GIMPRUBY_UNSTABLE
pluginexec_SCRIPTS = $(scripts) $(test_scripts)
else
pluginexec_SCRIPTS = $(scripts)
endif


EXTRA_DIST = $(scripts) $(test_scripts)
