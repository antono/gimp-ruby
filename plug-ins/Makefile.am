## Process this file with automake to produce Makefile.in

pluginexecdir = $(GIMP_PLUGINDIR)

scripts =  \
    coolmetal-logo.rb \
    sphere.rb   \
    hello.rb    \
    test.rb

test_scripts =  

 
if RUBYGIMP_UNSTABLE
pluginexec_SCRIPTS = $(scripts) $(test_scripts)
else
pluginexec_SCRIPTS = $(scripts)
endif


EXTRA_DIST = $(scripts) $(test_scripts)
