/* module.h -- dynamic module management: a wrapper for ltdl
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
#ifndef SIC_MODULE_H
#define SIC_MODULE_H 1

#include <sic/builtin.h>
#include <sic/common.h>
#include <sic/sic.h>

BEGIN_C_DECLS

typedef void ModuleInit         (Sic *sic);
typedef void ModuleFinish       (Sic *sic);

extern const char *module_error (void);
extern int module_init          (void);
extern int module_load          (Sic *sic, const char *name);
extern int module_unload        (Sic *sic, const char *name);

END_C_DECLS

#endif /* !SIC_MODULE_H */
/** @end 1 */
