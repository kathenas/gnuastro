/*********************************************************************
CosmicCalculator - Calculate cosmological parameters
CosmicCalculator is part of GNU Astronomy Utilities (Gnuastro) package.

Original author:
     Mohammad Akhlaghi <mohammad@akhlaghi.org>
Contributing author(s):
Copyright (C) 2016-2019, Free Software Foundation, Inc.

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
#include <config.h>

#include <argp.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>

#include <gsl/gsl_const_mksa.h>

#include <gnuastro/fits.h>
#include <gnuastro/table.h>

#include <gnuastro-internal/timing.h>
#include <gnuastro-internal/options.h>
#include <gnuastro-internal/checkset.h>
#include <gnuastro-internal/fixedstringmacros.h>

#include "main.h"

#include "ui.h"
#include "authors-cite.h"





/**************************************************************/
/*********      Argp necessary global entities     ************/
/**************************************************************/
/* Definition parameters for the Argp: */
const char *
argp_program_version = PROGRAM_STRING "\n"
                       GAL_STRINGS_COPYRIGHT
                       "\n\nWritten/developed by "PROGRAM_AUTHORS;

const char *
argp_program_bug_address = PACKAGE_BUGREPORT;

static char
args_doc[] = "";

const char
doc[] = GAL_STRINGS_TOP_HELP_INFO PROGRAM_NAME" will do cosmological "
  "calculations. If no redshfit is specified, it will only print the main "
  "input parameters. If only a redshift is given, it will print a table of "
  "all calculations. If any of the single row calculations are requested, "
  "only their values will be printed with a single space between each.\n"
  GAL_STRINGS_MORE_HELP_INFO
  /* After the list of options: */
  "\v"
  PACKAGE_NAME" home page: "PACKAGE_URL;




















/**************************************************************/
/*********    Initialize & Parse command-line    **************/
/**************************************************************/
static void
ui_initialize_options(struct cosmiccalparams *p,
                      struct argp_option *program_options,
                      struct argp_option *gal_commonopts_options)
{
  size_t i;
  struct gal_options_common_params *cp=&p->cp;

  /* Set the necessary common parameters structure. */
  cp->poptions           = program_options;
  cp->program_name       = PROGRAM_NAME;
  cp->program_exec       = PROGRAM_EXEC;
  cp->program_bibtex     = PROGRAM_BIBTEX;
  cp->program_authors    = PROGRAM_AUTHORS;
  cp->coptions           = gal_commonopts_options;

  /* Program specific initializations. */
  p->redshift            = NAN;

  /* Modify the common options. */
  for(i=0; !gal_options_is_last(&cp->coptions[i]); ++i)
    {
      /* Select by group. */
      switch(cp->coptions[i].group)
        {
        case GAL_OPTIONS_GROUP_OUTPUT:
        case GAL_OPTIONS_GROUP_TESSELLATION:
          cp->coptions[i].doc=NULL; /* Necessary to remove title. */
          cp->coptions[i].flags=OPTION_HIDDEN;
          break;
        }

      /* Select specific options. */
      switch(cp->coptions[i].key)
        {
        case GAL_OPTIONS_KEY_HDU:
        case GAL_OPTIONS_KEY_LOG:
        case GAL_OPTIONS_KEY_TYPE:
        case GAL_OPTIONS_KEY_QUIET:
        case GAL_OPTIONS_KEY_SEARCHIN:
        case GAL_OPTIONS_KEY_NUMTHREADS:
        case GAL_OPTIONS_KEY_IGNORECASE:
        case GAL_OPTIONS_KEY_TABLEFORMAT:
        case GAL_OPTIONS_KEY_STDINTIMEOUT:
          cp->coptions[i].flags=OPTION_HIDDEN;
          break;
        }
    }
}





/* Parse a single option: */
error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
  struct cosmiccalparams *p = state->input;

  /* Pass `gal_options_common_params' into the child parser.  */
  state->child_inputs[0] = &p->cp;

  /* In case the user incorrectly uses the equal sign (for example
     with a short format or with space in the long format, then `arg`
     start with (if the short version was called) or be (if the long
     version was called with a space) the equal sign. So, here we
     check if the first character of arg is the equal sign, then the
     user is warned and the program is stopped: */
  if(arg && arg[0]=='=')
    argp_error(state, "incorrect use of the equal sign (`=`). For short "
               "options, `=` should not be used and for long options, "
               "there should be no space between the option, equal sign "
               "and value");

  /* Set the key to this option. */
  switch(key)
    {

    /* Read the non-option tokens (arguments): */
    case ARGP_KEY_ARG:
      argp_error(state, "currently %s doesn't take any arguments",
                 PROGRAM_NAME);
      break;


    /* This is an option, set its value. */
    default:
      return gal_options_set_from_key(key, arg, p->cp.poptions, &p->cp);
    }

  return 0;
}





static void *
ui_add_to_single_value(struct argp_option *option, char *arg,
                      char *filename, size_t lineno, void *params)
{
  /* In case of printing the option values. */
  if(lineno==-1)
    error(EXIT_FAILURE, 0, "currently the options to be printed in one row "
          "(like `--age', `--luminositydist', and etc) do not support "
          "printing with the `--printparams' (`-P'), or writing into "
          "configuration files due to lack of time when implementing "
          "these features. You can put them into configuration files "
          "manually. Please get in touch with us at `%s', so we can "
          "implement it", PACKAGE_BUGREPORT);

  /* If this option is given in a configuration file, then `arg' will not
     be NULL and we don't want to do anything if it is `0'. */
  if(arg)
    {
      /* Make sure the value is only `0' or `1'. */
      if( arg[1]!='\0' && *arg!='0' && *arg!='1' )
        error_at_line(EXIT_FAILURE, 0, filename, lineno, "the `--%s' "
                      "option takes no arguments. In a configuration "
                      "file it can only have the values `1' or `0', "
                      "indicating if it should be used or not",
                      option->name);

      /* Only proceed if the (possibly given) argument is 1. */
      if(arg[0]=='0' && arg[1]=='\0') return NULL;
    }

  /* Add this option to the print list and return. */
  gal_list_i32_add(option->value, option->key);
  return NULL;
}




















/**************************************************************/
/***************       Sanity Check         *******************/
/**************************************************************/
/* Read and check ONLY the options. When arguments are involved, do the
   check in `ui_check_options_and_arguments'. */
static void
ui_read_check_only_options(struct cosmiccalparams *p)
{
  double sum = p->olambda + p->omatter + p->oradiation;

  /* Check if the density fractions add up to 1 (within floating point
     error). */
  if( sum > (1+1e-8) || sum < (1-1e-8) )
    error(EXIT_FAILURE, 0, "sum of fractional densities is not 1, but %.8f. "
          "The cosmological constant (`olambda'), matter (`omatter') "
          "and radiation (`oradiation') densities are given as %.8f, %.8f, "
          "%.8f", sum, p->olambda, p->omatter, p->oradiation);
}




















/**************************************************************/
/***************       Preparations         *******************/
/**************************************************************/
static void
ui_preparations(struct cosmiccalparams *p)
{
  /* The list is filled out in a first-in-last-out order. By the time
     control reaches here, the list is finalized. So we should just reverse
     it so the user gets values in the same order they requested them. */
  gal_list_i32_reverse(&p->specific);
}



















/**************************************************************/
/************         Set the parameters          *************/
/**************************************************************/

void
ui_read_check_inputs_setup(int argc, char *argv[], struct cosmiccalparams *p)
{
  struct gal_options_common_params *cp=&p->cp;


  /* Include the parameters necessary for argp from this program (`args.h')
     and for the common options to all Gnuastro (`commonopts.h'). We want
     to directly put the pointers to the fields in `p' and `cp', so we are
     simply including the header here to not have to use long macros in
     those headers which make them hard to read and modify. This also helps
     in having a clean environment: everything in those headers is only
     available within the scope of this function. */
#include <gnuastro-internal/commonopts.h>
#include "args.h"

  /* Initialize the options and necessary information.  */
  ui_initialize_options(p, program_options, gal_commonopts_options);


  /* Read the command-line options and arguments. */
  errno=0;
  if(argp_parse(&thisargp, argc, argv, 0, 0, p))
    error(EXIT_FAILURE, errno, "parsing arguments");


  /* Read the configuration files and set the common values. */
  gal_options_read_config_set(&p->cp);


  /* Read the options into the program's structure, and check them and
     their relations prior to printing. */
  ui_read_check_only_options(p);


  /* Print the option values if asked. Note that this needs to be done
     after the option checks so un-sane values are not printed in the
     output state. */
  gal_options_print_state(&p->cp);


  /* Read/allocate all the necessary starting arrays. */
  ui_preparations(p);
}
