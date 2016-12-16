/*********************************************************************
table -- functions for table input and output.
This is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <akhlaghi@gnu.org>
Contributing author(s):
Copyright (C) 2015, Free Software Foundation, Inc.

Gnuastro is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

Gnuastro is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef __GAL_TABLE_H__
#define __GAL_TABLE_H__

/* Include other headers if necessary here. Note that other header files
   must be included before the C++ preparations below */

#include <gnuastro/fits.h> /* Includes `gnuastro/data.h' and `fitsio.h' */
#include <gnuastro/linkedlist.h>



/* C++ Preparations */
#undef __BEGIN_C_DECLS
#undef __END_C_DECLS
#ifdef __cplusplus
# define __BEGIN_C_DECLS extern "C" {
# define __END_C_DECLS }
#else
# define __BEGIN_C_DECLS                /* empty */
# define __END_C_DECLS                  /* empty */
#endif
/* End of C++ preparations */





/* Actual header contants (the above were for the Pre-processor). */
__BEGIN_C_DECLS  /* From C++ preparations */





/* Types of table storage for input or output. */
enum gal_table_types
{
  GAL_TABLE_TYPE_TXT,                     /* Plain text table.  */
  GAL_TABLE_TYPE_AFITS,                   /* FITS ASCII table.  */
  GAL_TABLE_TYPE_BFITS,                   /* FITS binary table. */
};





/* When the desired column is not a number, should the string match or
   regular expression search be in the name, units or comments of the
   columns? */
enum gal_table_where_to_search
{
  GAL_TABLE_SEARCH_NAME,                   /* Match/search in names.    */
  GAL_TABLE_SEARCH_UNIT,                   /* Match/search in units.    */
  GAL_TABLE_SEARCH_COMMENT,                /* Match/search in comments. */
};





/* Functions */
gal_data_t *
gal_table_info(char *filename, char *hdu, size_t *numcols, int *tabletype);

int
gal_table_searchin_from_str(char *searchin_str);

gal_data_t *
gal_table_read_cols(char *filename, char *hdu,
                    struct gal_linkedlist_stll *cols, int searchin,
                    int ignorecase);


__END_C_DECLS    /* From C++ preparations */

#endif           /* __GAL_TABLE_H__ */