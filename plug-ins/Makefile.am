## Process this file with automake to produce Makefile.in

pluginexecdir = $(GIMP_PLUGINDIR)

scripts =  \
    coolmetal-logo.rb \
    sphere.rb

test_scripts = \
    hello.rb   \
    test.rb

 
if GIMPRUBY_UNSTABLE
pluginexec_SCRIPTS = $(scripts) $(test_scripts)
else
pluginexec_SCRIPTS = $(scripts)
endif


EXTRA_DIST = $(scripts) $(test_scripts)
