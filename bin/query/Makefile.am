## Process this file with automake to produce Makefile.inx
##
## Original author:
##     Mohammad akhlaghi <mohammad@akhlaghi.org>
## Contributing author(s):
## Copyright (C) 2016-2024 Free Software Foundation, Inc.
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


## Necessary pre-processer and linker flags.
AM_LDFLAGS  = -L\$(top_builddir)/lib
AM_CPPFLAGS = -I\$(top_builddir)/bootstrapped/lib \
              -I\$(top_srcdir)/bootstrapped/lib \
              -I\$(top_srcdir)/lib



## Program definition (name, linking, sources and headers)
bin_PROGRAMS = astquery

## Reason for linking with 'libgnu' described in 'bin/TEMPLATE/Makefile.am'.
astquery_LDADD = $(top_builddir)/bootstrapped/lib/libgnu.la \
                 $(top_builddir)/lib/libgnuastro.la \
                 $(CONFIG_LDADD)

astquery_SOURCES = main.c ui.c query.c astron.c gaia.c ned.c tap.c \
                   vizier.c

EXTRA_DIST = main.h authors-cite.h args.h ui.h query.h astron.h \
             gaia.h ned.h tap.h vizier.h



## The configuration file (distribute and install).
## NOTE: the man page is created in doc/Makefile.am
pkgsysconfdir = $(sysconfdir)/$(PACKAGE)
dist_pkgsysconf_DATA = astquery.conf
