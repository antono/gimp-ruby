# Configure check for Ruby
# Kenta MURATA <muraken2 / nifty.com>
# Modifications in June, 2006 by Kevin Cozens <kcozens@interlog.com>

dnl AM_CHECK_RUBY([MINIMUM-VERSION [, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for Ruby, and define RUBY_CFLAGS, RUBY_LIBS, RUBY_LDSHARED, RUBY_DLEXT,
dnl ruby_libdir, ruby_archdir, ruby_sitelibdir and ruby_sitearchdir
dnl
AC_DEFUN(AM_CHECK_RUBY,
[dnl
dnl Get the cflags and libraries from the rbconfig.rb
dnl

  save_CFLAGS="$CFLAGS"
  save_LIBS="$LIBS"

  AC_PATH_PROG(RUBY, ruby, no)
  if test "$RUBY" = "no"; then
    AC_MSG_ERROR([*** Could not find Ruby interpreter.])
  fi

  RUBY_BIN="$RUBY"

  minimum_version=ifelse([$1], ,1.4.4, $1)
  AC_MSG_CHECKING([for Ruby - version >= $minimum_version])
  ruby_major_version=`$RUBY -e "print RUBY_VERSION.split('.')[[0]]"`
  ruby_minor_version=`$RUBY -e "print RUBY_VERSION.split('.')[[1]]"`
  ruby_micro_version=`$RUBY -e "print RUBY_VERSION.split('.')[[2]]"`
  if `$RUBY -e "
      major_version = RUBY_VERSION.split('.')[[0]].to_i
      minor_version = RUBY_VERSION.split('.')[[1]].to_i
      micro_version = RUBY_VERSION.split('.')[[2]].to_i
      min_major_version = '$minimum_version'.split('.')[[0]].to_i
      min_minor_version = '$minimum_version'.split('.')[[1]].to_i
      min_micro_version = '$minimum_version'.split('.')[[2]].to_i
      if ((major_version > min_major_version) ||
          ((major_version == min_major_version) &&
           (minor_version > min_minor_version)) ||
          ((major_version == min_major_version) &&
           (minor_version == min_minor_version) &&
           (micro_version >= min_micro_version))) then
        exit 1
      end
      exit 0"`; then
    AC_MSG_RESULT([no])
    AC_MSG_ERROR([*** Ruby version is too old.])
  fi
  AC_MSG_RESULT([yes])

  ruby_libdir=`$RUBY -r rbconfig -e "puts Config::CONFIG[['rubylibdir']]"`
  ruby_archdir=`$RUBY -r rbconfig -e "puts Config::CONFIG[['archdir']]"`
  ruby_sitelibdir=`$RUBY -r rbconfig -e "puts Config::CONFIG[['sitelibdir']]"`
  ruby_sitearchdir=`$RUBY -r rbconfig -e "puts Config::CONFIG[['sitearchdir']]"`

  AC_CHECK_HEADER($ruby_archdir/ruby.h,,
    AC_MSG_ERROR([*** Can't find Ruby header files. Please install them.]))

  CFLAGS=`$RUBY -r rbconfig -e "puts Config::CONFIG[['CCDLFLAGS']]"`
  RUBY_CFLAGS="$RUBY_CFLAGS $CFLAGS"
  CFLAGS=`$RUBY -r rbconfig -e "puts Config::CONFIG[['CFLAGS']]"`
  RUBY_CFLAGS="$RUBY_CFLAGS $CFLAGS -I\$(ruby_archdir)";

  RUBY_LDSHARED=`$RUBY -r rbconfig -e "puts Config::CONFIG[['LDSHARED']]"`
  LIBS=`$RUBY -r rbconfig -e "puts Config::CONFIG[['LDFLAGS']]"`
  RUBY_LIBS="$RUBY_LIBS $LIBS"
  LIBS=`$RUBY -r rbconfig -e "puts Config::CONFIG[['DLDFLAGS']]"`
  RUBY_LIBS="$RUBY_LIBS $LIBS"
  RUBY_DLEXT=`$RUBY -r rbconfig -e "puts Config::CONFIG[['DLEXT']]"`

  CFLAGS="$save_CFLAGS"
  LIBS="$save_LIBS"

  AC_SUBST(RUBY_BIN)
  AC_SUBST(RUBY_CFLAGS)
  AC_SUBST(RUBY_LIBS)
  AC_SUBST(RUBY_DLEXT)
  AC_SUBST(RUBY_LDSHARED)
  AC_SUBST(ruby_libdir)
  AC_SUBST(ruby_archdir)
  AC_SUBST(ruby_sitelibdir)
  AC_SUBST(ruby_sitearchdir)
])
