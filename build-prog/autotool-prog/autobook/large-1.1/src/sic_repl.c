/* sic_repl.c -- read commands, evaluate them and print the results
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

#include "common.h"

#ifdef HAVE_LIBREADLINE
#  include <readline/readline.h>
#  include <readline/history.h>
#endif

#include "eval.h"
#include "sic_repl.h"

#ifndef BUFSIZ
#  define BUFSIZ 256
#endif

int is_interactive = 0;

static char *getline (FILE *in, const char *prompt);

int
source (Sic *sic, const char *path)
{
  FILE *stream;
  int result = SIC_OKAY;
  int save_interactive = is_interactive;

  SIC_ASSERT (sic && path);
  
  is_interactive = 0;

  if ((access (path, R_OK) < 0)
      || ((stream = fopen (path, "rt")) == NULL))
    {
      sic_result_clear (sic);
      sic_result_append (sic, "cannot source \"", path, "\": ",
			 strerror (errno), NULL);
      result = SIC_ERROR;
    }
  else
    result =  evalstream (sic, stream);

  is_interactive = save_interactive;

  return result;
}

int
evalstream (Sic *sic, FILE *stream)
{
  int status = SIC_EXIT;
  char *line;

  SIC_ASSERT (sic && stream);

  line = getline (stream, sicstate_get (sic, "PS1"));
  
  while (line)
    {
      status = evalline (sic, &line);

      switch (status)
	{
	case SIC_INCOMPLETE:
	  {
	    char *buf = xstrdup (line);
	    int len = strlen (buf);

	    line = getline (stream, sicstate_get (sic, "PS2"));
	    len += strlen (line);

	    buf = XREALLOC (char, buf, 2 + len);
	    strcat (buf, "\n");
	    strcat (buf, line);

	    line = buf;
	  }
	  break;

	case SIC_EXIT:
	  goto exit_sic;
	  /*NOBREAK*/
      
	case SIC_ERROR:
	  fprintf (stderr, "%s.\n", sic_result_get (sic));
	  /*NOBREAK*/

	default:
	  line = getline (stream, sicstate_get (sic, "PS1"));
	  break;
	}

      sic_result_clear (sic);
    }
  
  if (is_interactive && !line)
    printf ("\n");
  
 exit_sic:
  return status;
}

int
evalline (Sic *sic, char **pline)
{
  int status = SIC_OKAY;
  char *command = *pline;
  
  while (status != SIC_INCOMPLETE && *command)
    {
      Tokens *tokens;

      status = tokenize (sic, &tokens, &command);

      if ((status == SIC_OKAY)
	  && tokens && tokens->argv && tokens->argv[0] && *tokens->argv[0])
	status = eval (sic, tokens);

      if (tokens)
	{
	  free (*tokens->argv);
	  XFREE (tokens->argv);
	  XFREE (tokens);
	}
    }

  if (status != SIC_INCOMPLETE)
    *pline = command;
  
  return status;
}

static char *
getline (FILE *in, const char *prompt)
{
  int lim = BUFSIZ;
  int i = 0;
  int isdone = 0;

  static char *buf = NULL;	/* Always allocated and freed
				   from inside this function.  */
  
  XFREE (buf);

  if (is_interactive)
    {
      buf = (char *) readline ((char *) prompt);
      
#ifdef HAVE_ADD_HISTORY
      if (buf && *buf)
        add_history (buf);
#endif
      
      return buf;
    }

  buf = XMALLOC (char, lim);
      
  while (!isdone)
    {
      int c = getc (in);

      switch (c)
	{
	case EOF:
	  isdone = EOF;
	  break;

	case '\n':
	  isdone = 1;
	  break;
	  
	default:
	  if (i == lim)
	    {
	      lim *= 2;
	      buf = XREALLOC (char, buf, lim);
	    }
	  buf[i++] = (char)c;
	  break;
	}
    }
  buf[i] = 0;

  return isdone == EOF ? NULL : buf;
}
