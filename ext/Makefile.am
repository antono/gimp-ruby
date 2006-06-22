## Process this file with automake to produce Makefile.in

if PLATFORM_WIN32
no_undefined = -no-undefined
path_separator = ;
else
path_separator = :
endif

libdir = $(GIMP_LIBDIR)/ruby

AM_CFLAGS = $(RUBY_CFLAGS) $(GIMP_CFLAGS)

lib_LTLIBRARIES = gimpext.la

gimpext_la_SOURCES = \
    conversion.c         \
    ids.c                \
    rbgimp.c             \
    rbgimpcolorcmyk.c    \
    rbgimpcolorhsv.c     \
    rbgimpcolorrgb.c     \
    rbgimpcolorspace.c   \
    rbgimpconstants.c    \
    rbgimp.h             \
    rbgimpmain.c         \
    rbgimpproceduraldb.c \
    structures.c

gimpext_la_LDFLAGS = -module -avoid-version -shrext $(RUBY_DLEXT) \
    $(no_undefined)

gimpext_la_LIBADD = \
    $(GIMP_LIBS) \
    $(RUBY_LIBS)
