## Process this file with automake to produce Makefile.in
##
## Original author:
##     Mohammad Akhlaghi <mohammad@akhlaghi.org>
## Contributing author(s):
## Copyright (C) 2015-2024 Free Software Foundation, Inc.
##
## Gnuastro is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## Gnuastro is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Gnuastro. If not, see <http://www.gnu.org/licenses/>.




# Conditional compilation
headersdir=$(top_srcdir)/lib/gnuastro
if COND_HASWCSDIS_H
  MAYBE_WCSDISTORTION = wcsdistortion.c
endif
if COND_HASGNUMAKE_H
  MAYBE_GNUMAKE = libgnuastro_make.la
endif
if COND_NUMPY
  MAYBE_NUMPY_C = python.c
  MAYBE_NUMPY_H = $(headersdir)/python.h
  MAYBE_NUMPY_INCLUDE = -I$(NUMPY_INCLUDE_DIR) -I$(PYTHON_INCLUDE_DIR)
endif





## Necessary flags.
##
##   $(top_builddir)/bootstrapped/lib: Gnulib headers that are customized
##   for the running system.
##
##   $(top_srcdir)/bootstrapped/lib: Gnulib headers that are generic (don't
##   need any customization).
##
##   SYSCONFIG_DIR: only necessary in 'options.c' to get the system
##       installation directory.
##
##   MAYBE_NUMPY_INCLUDE: only necessary in 'python.c' to link the
##                        numpy C-API.
AM_CPPFLAGS = -I\$(top_builddir)/bootstrapped/lib \
              -I\$(top_srcdir)/bootstrapped/lib \
              -DSYSCONFIG_DIR=\"$(sysconfdir)/$(PACKAGE)\" \
              $(MAYBE_NUMPY_INCLUDE)





# Define the main compiled library file, its Libtool version and also link
# with Gnulib's compiled library for this system. We are doing this so the
# Gnuastro library functions can also benefit from Gnulib's many great
# features. This also avoids the need for the programs to link separately
# with Gnulib, they only need to link with the Gnuastro library.
lib_LTLIBRARIES = libgnuastro.la $(MAYBE_GNUMAKE)

# Linking flags for the Gnuastro library.
libgnuastro_la_LIBADD = $(top_builddir)/bootstrapped/lib/libgnu.la
libgnuastro_la_LDFLAGS = -version-info $(GAL_LT_VERSION) $(CONFIG_LDADD) \
                         -lc -no-undefined

# Gnuastro's GNU Make extensions
libgnuastro_make_la: libgnuastro.la
libgnuastro_make_la_LIBADD = libgnuastro.la
libgnuastro_make_la_LDFLAGS = -version-info $(GAL_LT_VERSION)



# Specify the library .c files
libgnuastro_make_la_SOURCES = makeplugin.c
libgnuastro_la_SOURCES = \
  $(MAYBE_NUMPY_C) \
  $(MAYBE_WCSDISTORTION) \
  arithmetic.c \
  arithmetic-and.c \
  arithmetic-bitand.c \
  arithmetic-bitlsh.c \
  arithmetic-bitor.c \
  arithmetic-bitrsh.c \
  arithmetic-bitxor.c \
  arithmetic-divide.c \
  arithmetic-eq.c\
  arithmetic-ge.c \
  arithmetic-gt.c \
  arithmetic-le.c \
  arithmetic-lt.c \
  arithmetic-minus.c \
  arithmetic-modulo.c \
  arithmetic-multiply.c \
  arithmetic-ne.c \
  arithmetic-or.c \
  arithmetic-plus.c \
  arithmetic-set.c \
  array.c \
  binary.c \
  blank.c \
  box.c \
  checkset.c \
  color.c \
  convolve.c \
  cosmology.c \
  data.c \
  ds9.c \
  eps.c \
  fit.c \
  fits.c \
  git.c \
  interpolate.c \
  jpeg.c \
  kdtree.c \
  label.c \
  list.c \
  match.c \
  options.c \
  pdf.c \
  permutation.c \
  pointer.c \
  polygon.c \
  pool.c \
  qsort.c \
  dimension.c \
  speclines.c \
  statistics.c \
  table.c \
  tableintern.c \
  threads.c \
  tiff.c \
  tile.c \
  tile-internal.c \
  timing.c \
  txt.c \
  type.c \
  units.c \
  warp.c \
  wcs.c






# Installed headers, note that we are not blindly including all '.h' files
# in the $(headersdir) directory. Some of the header files don't need to be
# installed.
pkginclude_HEADERS = gnuastro/config.h \
  $(MAYBE_NUMPY_H) \
  $(headersdir)/arithmetic.h \
  $(headersdir)/array.h \
  $(headersdir)/binary.h \
  $(headersdir)/blank.h \
  $(headersdir)/box.h \
  $(headersdir)/color.h \
  $(headersdir)/convolve.h \
  $(headersdir)/cosmology.h \
  $(headersdir)/data.h \
  $(headersdir)/dimension.h \
  $(headersdir)/ds9.h \
  $(headersdir)/eps.h \
  $(headersdir)/fit.h \
  $(headersdir)/fits.h \
  $(headersdir)/git.h \
  $(headersdir)/interpolate.h \
  $(headersdir)/jpeg.h \
  $(headersdir)/kdtree.h \
  $(headersdir)/label.h \
  $(headersdir)/list.h \
  $(headersdir)/match.h \
  $(headersdir)/pdf.h \
  $(headersdir)/permutation.h \
  $(headersdir)/pointer.h \
  $(headersdir)/polygon.h \
  $(headersdir)/pool.h \
  $(headersdir)/qsort.h \
  $(headersdir)/speclines.h \
  $(headersdir)/statistics.h \
  $(headersdir)/table.h \
  $(headersdir)/threads.h \
  $(headersdir)/tiff.h \
  $(headersdir)/tile.h \
  $(headersdir)/txt.h \
  $(headersdir)/type.h \
  $(headersdir)/units.h \
  $(headersdir)/warp.h \
  $(headersdir)/wcs.h





# Files to distribute in the tarball. These are internal headers and don't
# need to be installed. Headers are only mentioned within the source files,
# and if they are not explicitly mentioned somewhere in the Makefile, they
# will not distributed, so we need to explicitly tell Automake to
# distribute them here.
internaldir=$(top_srcdir)/lib/gnuastro-internal
EXTRA_DIST = gnuastro.pc.in \
  $(headersdir)/README \
  $(internaldir)/README \
  $(internaldir)/arithmetic-and.h \
  $(internaldir)/arithmetic-binary.h \
  $(internaldir)/arithmetic-bitand.h  \
  $(internaldir)/arithmetic-bitlsh.h \
  $(internaldir)/arithmetic-bitor.h  \
  $(internaldir)/arithmetic-bitrsh.h \
  $(internaldir)/arithmetic-bitxor.h \
  $(internaldir)/arithmetic-divide.h \
  $(internaldir)/arithmetic-eq.h  \
  $(internaldir)/arithmetic-ge.h \
  $(internaldir)/arithmetic-gt.h  \
  $(internaldir)/arithmetic-internal.h \
  $(internaldir)/arithmetic-le.h  \
  $(internaldir)/arithmetic-lt.h \
  $(internaldir)/arithmetic-minus.h  \
  $(internaldir)/arithmetic-modulo.h \
  $(internaldir)/arithmetic-multiply.h  \
  $(internaldir)/arithmetic-ne.h \
  $(internaldir)/arithmetic-or.h  \
  $(internaldir)/arithmetic-plus.h \
  $(internaldir)/arithmetic-set.h  \
  $(internaldir)/checkset.h \
  $(internaldir)/commonopts.h  \
  $(internaldir)/config.h.in \
  $(internaldir)/fixedstringmacros.h  \
  $(internaldir)/options.h \
  $(internaldir)/tableintern.h  \
  $(internaldir)/tile-internal.h \
  $(internaldir)/timing.h  \
  $(internaldir)/wcsdistortion.h





# Definitions for Gnuastro's the pkg-config file (inspired from GSL's
# Makefile.am)
pkgconfig_DATA = gnuastro.pc
pkgconfigdir = $(libdir)/pkgconfig
CLEANFILES = gnuastro.pc gnuastro/config.h





# Build 'gnuastro/config.h' based on the information in the Makefile after
# the Makefile has been built.
gnuastro/config.h: Makefile $(internaldir)/config.h.in
	rm -f $@ $@.tmp
	$(MKDIR_P) gnuastro
	$(SED) -e 's|@VERSION[@]|$(VERSION)|g' \
	       -e 's|@HAVE_PYTHON[@]|$(HAVE_PYTHON)|g' \
	       -e 's|@SIZEOF_LONG[@]|$(SIZEOF_LONG)|g' \
	       -e 's|@SIZEOF_SIZE_T[@]|$(SIZEOF_SIZE_T)|g' \
	       -e 's|@HAVE_GNUMAKE_H[@]|$(HAVE_GNUMAKE_H)|g' \
	       -e 's|@HAVE_GSL_STEFFEN[@]|$(HAVE_GSL_STEFFEN)|g' \
	       -e 's|@HAVE_LIBGIT2[@]|$(HAVE_LIBGIT2_FOR_CONF)|g' \
	       -e 's|@HAVE_WCSLIB_DIS_H[@]|$(HAVE_WCSLIB_DIS_H)|g' \
	       -e 's|@HAVE_WCSLIB_MJDREF[@]|$(HAVE_WCSLIB_MJDREF)|g' \
	       -e 's|@HAVE_WCSLIB_OBSFIX[@]|$(HAVE_WCSLIB_OBSFIX)|g' \
	       -e 's|@HAVE_WCSLIB_WCSCCS[@]|$(HAVE_WCSLIB_WCSCCS)|g' \
	       -e 's|@HAVE_WCSLIB_VERSION[@]|$(HAVE_WCSLIB_VERSION)|g' \
	       -e 's|@HAVE_PTHREAD_BARRIER[@]|$(HAVE_PTHREAD_BARRIER)|g' \
	       -e 's|@RESTRICT_REPLACEMENT[@]|$(RESTRICT_REPLACEMENT)|g' \
	       -e 's|@HAVE_FITS_IS_REENTRANT[@]|$(HAVE_FITS_IS_REENTRANT)|g' \
               $(internaldir)/config.h.in >> $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@





# Build Gnuastro's pkg-config file similar to 'gnuastro/config.h'.
gnuastro.pc: Makefile $(srcdir)/gnuastro.pc.in
	rm -f $@ $@.tmp
	ol=""; \
	if [ x"$(HAVE_LIBJPEG)" = xyes ]; then ol="$$ol libjpeg"; fi; \
	if [ x"$(HAVE_LIBLZMA)" = xyes ]; then ol="$$ol liblzma"; fi; \
	if [ x"$(HAVE_LIBGIT2)" = xyes ]; then ol="$$ol libgit2"; fi; \
	if [ x"$(HAVE_LIBTIFF)" = xyes ]; then ol="$$ol libtiff-4"; fi; \
	$(SED) -e's|@prefix[@]|$(prefix)|g' \
	       -e"s|@optional_libs[@]|$$ol|g" \
	       -e's|@exec_prefix[@]|$(exec_prefix)|g' \
	       -e's|@libdir[@]|$(libdir)|g' \
	       -e's|@includedir[@]|$(includedir)|g' \
	       -e's|@LIBS[@]|$(LIBS)|g' \
	       -e's|@VERSION[@]|$(VERSION)|g' \
	       '$(srcdir)/$@.in' >> $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@
