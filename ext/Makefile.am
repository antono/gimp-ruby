## Process this file with automake to produce Makefile.in

if PLATFORM_WIN32
no_undefined = -no-undefined
path_separator = ;
else
path_separator = :
endif

libdir = $(GIMP_LIBDIR)/ruby

AM_CFLAGS = $(RUBY_CFLAGS) $(GIMP_CFLAGS) \
    -DLOCALEDIR=\""$(GIMP_LOCALEDIR)"\"

lib_LTLIBRARIES = gimpext.la

gimpext_la_SOURCES = \
    conversion.c         \
    definitions.c        \
    rbgimp.c             \
    rbgimpcolorrgb.c     \
    rbgimpconstants.c    \
    rbgimpdialog.c       \
    rbgimp.h             \
    rbgimpmain.c         \
    rbgimpproceduraldb.c 
    
#    rbgimpcolorspace.c

gimpext_la_LDFLAGS = -module -avoid-version -shrext $(RUBY_DLEXT) \
    $(no_undefined)

gimpext_la_LIBADD = \
    $(GIMP_LIBS) \
    $(RUBY_LIBS)

noinst_PROGRAMS = ruby-fu-console

ruby_fu_console_SOURCES = ruby-fu-console.c

ruby_fu_console_LDADD = \
    $(GIMP_LIBS)
