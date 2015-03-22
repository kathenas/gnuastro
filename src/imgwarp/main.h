/*********************************************************************
ImageWarp - Warp images using projective mapping.
ImageWarp is part of GNU Astronomy Utilities (gnuastro) package.

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
along with gnuastro. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>

#include "commonparams.h"



/* IMPORTANT NOTE:

Unlike most other programs, in ImageWarp, we will assume the
coordinate (0.0f, 0.0f) is on the bottom left corner of the first
image, not in the center of it.

*/


/* Progarm name macros: */
#define SPACK_VERSION   "0.1"
#define SPACK           "astimgwarp" /* Subpackage executable name. */
#define SPACK_NAME      "ImageWarp"  /* Subpackage full name.       */
#define SPACK_STRING    SPACK_NAME" ("PACKAGE_STRING") "SPACK_VERSION
#define LOGFILENAME     SPACK".log"







struct uiparams
{
  char        *inputname;  /* Name of input file.                      */
  char       *matrixname;  /* Name of transform file.                  */
  char     *matrixstring;  /* String containing transform elements.    */

  int    matrixstringset;
};





struct imgwarpparams
{
  /* Other structures: */
  struct uiparams     up;  /* User interface parameters.                 */
  struct commonparams cp;  /* Common parameters.                         */

  /* Input: */
  double           *input;  /* Name of input FITS file.                  */
  double          *matrix;  /* Warp/Transformation matrix.               */
  size_t              is0;  /* Number of rows in input image.            */
  size_t              is1;  /* Number of columns in input image.         */
  size_t              ms0;  /* Matrix number of rows.                    */
  size_t              ms1;  /* Matrix number of columns.                 */
  int         inputbitpix;  /* The type of the input array.              */

  /* Internal parameters: */
  double          *output;  /* Warped image array.                       */
  size_t              os0;  /* Output number of rows.                    */
  size_t              os1;  /* Output number of columns.                 */
  double         *inverse;  /* Inverse of the input matrix.              */
  time_t          rawtime;  /* Starting time of the program.             */
  size_t     oplygncrn[5];  /* Ordered PoLYGon CoRNers after inverse.    */
                            /* See explanation in orderedpolygoncorners. */
  double    outfpixval[2];  /* Value at bottom left position of output's */
};                          /* first pixel in output coordinates.        */

#endif