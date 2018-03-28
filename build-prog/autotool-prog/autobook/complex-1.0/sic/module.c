/* module.c -- dynamic module management: a wrapper for ltdl
   Copyright (C) 2000 Gary V. Vaughan
  
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/** @start 1 */
#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include "common.h"
#include "builtin.h"
#include "eval.h"
#include "ltdl.h"
#include "module.h"
#include "sic.h"

#ifndef SIC_MODULE_PATH_ENV
#  define SIC_MODULE_PATH_ENV   "SIC_MODULE_PATH"
#endif

/** @end 1 */
/** @start 4 */
static int unload_ltmodule (lt_dlhandle module, lt_ptr_t data);

/** @end 4 */
/** @start 5 */
static int userdata_address_compare (List *elt, void *match);

/** @end 5 */
/** @start 2 */
static char multi_init_error[]
            = "module loader initialised more than once";
static char no_builtin_table_error[]
            = "module has no builtin or syntax table";
static char builtin_unload_error[]
            = "builtin table failed to unload";
static char syntax_unload_error[]
            = "syntax table failed to unload";
static char module_not_found_error[]
            = "no such module";
static char module_not_unloaded_error[]
            = "module not unloaded";

static const char *last_error = NULL;

const char *
module_error (void)
{
  return last_error;
}

/** @end 2 */
/** @start 1 */
int
module_init (void)
{
  static int initialised = 0;
  int errors = 0;

  /* Only perform the initialisation once. */
  if (!initialised)
    {
      /* ltdl should use the same mallocation as us. */
      lt_dlmalloc = (lt_ptr_t (*) (size_t)) xmalloc;
      lt_dlfree = (void (*) (lt_ptr_t)) free;

      /* Make sure preloaded modules are initialised. */
      LTDL_SET_PRELOADED_SYMBOLS();

      last_error = NULL;

      /* Call ltdl initialisation function. */
      errors = lt_dlinit();


      /* Set up the module search directories. */
      if (errors == 0)
        {
          const char *path = getenv (SIC_MODULE_PATH_ENV);

          if (path != NULL)
            errors = lt_dladdsearchdir(path);
        }

      if (errors == 0)
        errors = lt_dladdsearchdir(MODULE_PATH);

      if (errors != 0)
        last_error = lt_dlerror ();

      ++initialised;

      return errors ? SIC_ERROR : SIC_OKAY;
    }

  last_error = multi_init_error;
  return SIC_ERROR;
}
/** @end 1 */
/** @start 3 */
int
module_load (Sic *sic, const char *name)
{
  lt_dlhandle module;
  Builtin *builtin_table;
  Syntax *syntax_table;
  int status = SIC_OKAY;

  last_error = NULL;

  module = lt_dlopenext (name);

  if (module)
    {
      builtin_table = (Builtin*) lt_dlsym (module, "builtin_table");
      syntax_table = (Syntax *) lt_dlsym (module, "syntax_table");
      if (!builtin_table && !syntax_table)
        {
          lt_dlclose (module);
          last_error = no_builtin_table_error;
          module = NULL;
        }
    }

  if (module)
    {
      ModuleInit *init_func
        = (ModuleInit *) lt_dlsym (module, "module_init");
      if (init_func)
          (*init_func) (sic);
    }

  if (module)
    {
      SyntaxFinish *syntax_finish
        = (SyntaxFinish *) lt_dlsym (module, "syntax_finish");
      SyntaxInit *syntax_init
        = (SyntaxInit *) lt_dlsym (module, "syntax_init");

      if (syntax_finish)
        sic->syntax_finish = list_cons (list_new (syntax_finish),
                                        sic->syntax_finish);
      if (syntax_init)
        sic->syntax_init = list_cons (list_new (syntax_init),
                                      sic->syntax_init);
    }

  if (module)
    {
      if (builtin_table)
        status = builtin_install (sic, builtin_table);

      if (syntax_table && status == SIC_OKAY)
        status = syntax_install (sic, module, syntax_table);

      return status;
    }

  last_error = lt_dlerror();
  if (!last_error)
    last_error = module_not_found_error;

  return SIC_ERROR;
}
/** @end 3 */
/** @start 4 */
struct unload_data { Sic *sic; const char *name; };

int
module_unload (Sic *sic, const char *name)
{
  struct unload_data data;

  last_error = NULL;

  data.sic = sic;
  data.name = name;

  /* Stopping might be an error, or we may have unloaded the module. */
  if (lt_dlforeach (unload_ltmodule, (lt_ptr_t) &data) != 0)
    if (!last_error)
      return SIC_OKAY;

  if (!last_error)
    last_error = module_not_found_error;
    
  return SIC_ERROR;
}
/** @end 4 */
/** @start 5 */
/* This callback returns `0' if the module was not yet found.
   If there is an error, LAST_ERROR will be set, otherwise the
   module was successfully unloaded. */
static int
unload_ltmodule (lt_dlhandle module, void *data)
{
  struct unload_data *unload = (struct unload_data *) data;
  const lt_dlinfo *module_info = lt_dlgetinfo (module);

  if ((unload == NULL)
      || (unload->name == NULL)
      || (module_info == NULL)
      || (module_info->name == NULL)
      || (strcmp (module_info->name, unload->name) != 0))
    {
      /* No match, return 0 to keep searching */
      return 0;
    }
  
  if (module)
    {
      /* Fetch the addresses of the entrypoints into the module. */
      Builtin *builtin_table
        = (Builtin*) lt_dlsym (module, "builtin_table");
      Syntax *syntax_table
        = (Syntax *) lt_dlsym (module, "syntax_table");
      void *syntax_init_address
        = (void *) lt_dlsym (module, "syntax_init");
      void **syntax_finish_address
        = (void *) lt_dlsym (module, "syntax_finish");
      List *stale;

      /* Remove all references to these entry points in the internal
         data structures, before actually unloading the module. */
      stale = list_remove (&unload->sic->syntax_init,
                   syntax_init_address, userdata_address_compare);
      XFREE (stale);
        
      stale = list_remove (&unload->sic->syntax_finish,
                   syntax_finish_address, userdata_address_compare);
      XFREE (stale);

      if (builtin_table
          && builtin_remove (unload->sic, builtin_table) != SIC_OKAY)
        {
          last_error = builtin_unload_error;
          module = NULL;
        }

      if (syntax_table
          && SIC_OKAY != syntax_remove (unload->sic, module,
					syntax_table))
        {
          last_error = syntax_unload_error;
          module = NULL;
        }
    }
  
  if (module)
    {
      ModuleFinish *finish_func
        = (ModuleFinish *) lt_dlsym (module, "module_finish");

      if (finish_func)
        (*finish_func) (unload->sic);
    }

  if (module)
    {
      if (lt_dlclose (module) != 0)
        module = NULL;
    }

  /* No errors?  Stop the search! */
  if (module)
    return 1;
      
  /* Find a suitable diagnostic. */
  if (!last_error)
    last_error = lt_dlerror();
  if (!last_error)
    last_error = module_not_unloaded_error;
      
  /* Error diagnosed.  Stop the search! */
  return -1;
}

static int
userdata_address_compare (List *elt, void *match)
{
  return (int) (elt->userdata - match);
}
/** @end 5 */
