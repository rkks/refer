/* dollar.c -- variables and '$' substitution
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

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <ctype.h>
#include <sys/wait.h>
#include <sic/module.h>

#include "sic_repl.h"

#define builtin_table		dollar_LTX_builtin_table
#define syntax_table		dollar_LTX_syntax_table

/* Generate prototypes. */
SIC_BUILTIN(builtin_set);
SIC_BUILTIN(builtin_unset);

Builtin builtin_table[] = {
  { "set",	builtin_set,	2, 2 },
  { "unset",	builtin_unset,	1, -1 },
  { NULL, NULL, 0, 0 }
};

/* Generate prototype. */
SIC_SYNTAX(syntax_subst);

Syntax syntax_table[] = {
  { syntax_subst,	"$" },
  { NULL,		NULL }
};

static char *extract_variable  (Sic *sic, BufferIn *in);
static char *extract_substcmd  (Sic *sic, BufferIn *in);
static int substitute	       (Sic *sic, BufferIn *in, char **pvalue);
static int execute_and_collect (Sic *sic, const char *command,
				char **pvalue);

BUILTIN_DECLARATION (set)
{
  int status = sicstate_set (sic, argv[1], xstrdup (argv[2]), free);

  if (status != SIC_OKAY)
    {
      sic_result_clear (sic);
      sic_result_append (sic, "\"", argv[1], "\" not set", NULL);
    }
    
  return status;
}

BUILTIN_DECLARATION (unset)
{
  int status = SIC_OKAY;
  int i, errors = 0;

  for (i = 1; argv[i]; ++i)
    if (sicstate_clear (sic, argv[i]) != SIC_OKAY)
      {
	if (errors++ > 0)
	  sic_result_append (sic, ", ", NULL);
	else
	  sic_result_clear (sic);

	sic_result_append (sic, "unbound variable \"", argv[1], "\"", NULL);
	status = SIC_ERROR;
      }

  return status;
}

SYNTAX_DECLARATION (subst)
{
  char *value = NULL;
  int status = substitute (sic, in, &value);

  if (value)
    {
      size_t len = strlen (value);

      while (out->buf.i + len >= out->buf.lim)
	{
	  out->buf.lim *= 2;
	  out->buf.start = XREALLOC (char, out->buf.start, out->buf.lim);
	}
      strcpy (&out->buf.start[out->buf.i], value);
      out->buf.i += len;

      XFREE (value);
    }

  return status;
}

static int
substitute (Sic *sic, BufferIn *in, char **pvalue)
{
  int status = SIC_ERROR;
  
  switch (in->buf.start[1 + in->buf.i])
    {
    case '(': /* command substitution */
      {
	char *substcmd = extract_substcmd (sic, in);
	status = execute_and_collect (sic, substcmd, pvalue);
	XFREE (substcmd);
      }
      break;
		    
    default:  /* variable substitution */
      {
	char *name = extract_variable (sic, in);
	char *value;

	if (!name)
	  {
	    sic_result_clear (sic);
	    sic_result_append (sic, "syntax error near \"",
			       &in->buf.start[in->buf.i], "\"", NULL);
	    break;
	  }
	
	value = sicstate_get (sic, name);
	if (value == NULL)
	  {
	    sic_result_clear (sic);
	    sic_result_append (sic, "\"", name,
			       "\" is undefined", NULL);
	    XFREE (name);
	    break;
	  }
	else
	  *pvalue = xstrdup (value);

	status = SIC_CONTINUE;
      }
      break;
    }

  return status;
}

static char *
extract_variable (Sic *sic, BufferIn *in)
{
  char *ptr = &in->buf.start[in->buf.i];
  char *name;
  int len = 1;

  while (ptr[len] && syntax_handler (sic, ptr[len]) == 0)
    ++len;
  name = strncpy (xmalloc (len), 1 + ptr, len -1);
  name[len -1] = 0;
  in->buf.i += len;
  
  return name;
}

static char *
extract_substcmd (Sic *sic, BufferIn *in)
{
  int *pinstring = sicstate_get (sic, "::syntax::instring");
  char *substcmd = NULL;
  int paren;
  int len;

  for (len = 2, paren = 1; paren; ++len)
    switch (in->buf.start[in->buf.i + len])
      {
      case '\"': *pinstring = !*pinstring; break;
      case '\\': ++len; break;
      case '(': if (!*pinstring) ++paren; break;
      case ')': if (!*pinstring) --paren; break;
      }

  substcmd = strncpy (xmalloc (len -2), &in->buf.start[2 + in->buf.i], len -3);
  substcmd[len -3] = 0;
  in->buf.i += len;

  return substcmd;
}

static int
execute_and_collect (Sic *sic, const char *line, char **pvalue)
{
  int status = SIC_OKAY;
  int fildes[2];
  int pid;

  SIC_ASSERT (line);

  pipe (fildes);
  pid = fork ();
  switch (pid)
    {
    case -1:			/* fork failed */
      status = SIC_ERROR;
      sic_result_set (sic, "fork failed", -1);
      break;

    case 0:			/* in child process */
      dup2 (fildes[1], 1);

      status = evalline (sic, (char **) &line);
		      
      close (fildes[0]);
      close (fildes[1]);
      if (status != SIC_OKAY)
	exit (EXIT_FAILURE);
      exit (EXIT_SUCCESS);
      break;

    default:			/* in parent process */
      {
	int stat;

	close (fildes[1]);
	if (waitpid (pid, &stat, 0) == pid)
	  {
	    if (WIFSIGNALED (stat))
	      {
		sic_result_clear (sic);
		sic_result_append (sic, "child \"", line,
				   "\" exited due to uncaught signal `",
				   strsignal (WTERMSIG (stat)), "'", NULL);
		status = SIC_ERROR;
	      }
	    else if (WIFEXITED (stat) && (WEXITSTATUS (stat) != 0))
	      {
		char exitstatus[4];
		sprintf (exitstatus, "%d", WEXITSTATUS (stat));
		sic_result_clear (sic);
		sic_result_append (sic, "child \"", line,
				   "\" exited abnormally with status `",
				   exitstatus, "'", NULL);
		status = SIC_ERROR;
	      }
	  }
	else
	  status = SIC_ERROR;
      }
      break;
    }

  if (status == SIC_OKAY)
    {
      int src = 0, dest  = 0, lim = 128, n = 0;
      char buf[128];

      *pvalue = XMALLOC (char, lim);
      for (;;)
	{
	  if (--n <= 0)
	    {
	      n = read (fildes[0], buf, sizeof(buf));
	      if (n <= 0)
		break;
	      src = 0;
	    }

	  if (dest +1 >= lim)
	    {
	      lim *= 2;
	      *pvalue = XREALLOC (char, *pvalue, lim);
	    }
		
	  (*pvalue)[dest++] = buf[src++];
	}
      /* strip trailing newline */
      if ((*pvalue)[dest -1] == '\n')
	--dest;
      (*pvalue)[dest] = 0;
    }

  return (status == SIC_OKAY) ? SIC_CONTINUE : status;
}
