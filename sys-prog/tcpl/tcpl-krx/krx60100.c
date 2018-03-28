/* K&R 6-1: "Our version of getword() does not properly handle
   underscores, string constants, or preprocessor control lines.
   Write a better version."

   This is intended to be a solution to K&R 6-1 in "category 0" as
   defined by the official rules given on Richard Heathfield's "The C
   Programming Language Answers To Exercises" page, found at
   http://users.powernet.co.uk/eton/kandr2/index.html.

   For more information on the language for which this is a lexical
   analyzer, please see the comment preceding getword() below.

   Note that there is a small modification to ungetch() as defined by
   K&R.  Hopefully this lies within the rules. */

/* knr61.c - answer to K&R2 exercise 6-1.
   Copyright (C) 2000 Ben Pfaff <blp@gnu.org>.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA. */
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokens.  Other non-whitespace characters self-represent themselves
   as tokens. */
enum token
  {
    TOK_ID = UCHAR_MAX + 1,     /* Identifier. */
    TOK_STRING,                 /* String constant. */
    TOK_CHAR,                   /* Character constant. */
    TOK_EOF                     /* End of file. */
  };

enum token getword (char *word, int lim);

static int skipws (void);
static int getstelem (char **, int *, int);
    
static int getch (void);
static void ungetch (int);
static void putch (char **, int *, int);

/* Main program for testing. */
int
main (void)
{
  ungetch ('\n');
  
  for (;;)
    {
      char word[64];
      enum token token;

      /* Get token. */
      token = getword (word, sizeof word);

      /* Print token type. */
      switch (token)
        {
        case TOK_ID:
          printf ("id");
          break;

        case TOK_STRING:
          printf ("string");
          break;

        case TOK_CHAR:
          printf ("char");
          break;

        case TOK_EOF:
          printf ("eof\n");
          return 0;

        default:
          printf ("other");
          word[0] = token;
          word[1] = '\0';
          break;
        }

      /* Print token value more or less unambiguously. */
      {
        const char *s;

        printf ("\t'");
        for (s = word; *s != '\0'; s++)
          if (isprint (*s) && *s != '\'')
            putchar (*s);
          else if (*s == '\'')
            printf ("\\'");
          else
            /* Potentially wrong. */
            printf ("\\x%02x", *s);
        printf ("'\n");
      }
    }
}

/* Parses C-like tokens from stdin:

        - Parses C identifiers and string and character constants.

        - Other characters, such as operators, punctuation, and digits
          not part of identifiers are considered as tokens in
          themselves.

        - Skip comments and preprocessor control lines.

   Does not handle trigraphs, line continuation with \, or numerous
   other special C features.

   Returns a token type.  This is either one of TOK_* above, or a single
   character in the range 0...UCHAR_MAX.

   If TOK_ID, TOK_STRING, or TOK_CHAR is returned, WORD[] is filled
   with the identifier or string value, truncated at LIM - 1
   characters and terminated with '\0'.

   For other returned token types, WORD[] is indeterminate. */
enum token
getword (char *word, int lim)
{
  int beg_line, c;
  
  for (;;)
    {
      beg_line = skipws ();
      c = getch ();

      if (!beg_line || c != '#')
        break;
      
      /* Skip preprocessor directive. */
      do
        {
          c = getch ();
          if (c == EOF)
            return TOK_EOF;
        }
      while (c != '\n');
      ungetch ('\n');
    }
  
  if (c == EOF)
    return TOK_EOF;
  else if (c == '_' || isalpha ((unsigned char) c))
    {
      do
        {
          putch (&word, &lim, c);
          c = getch ();
        }
      while (isalnum ((unsigned char) c) || c == '_');

      ungetch (c);
      return TOK_ID;
    }
  else if (c == '\'' || c == '"')
    {
      int quote = c;
      word[0] = '\0';
      while (getstelem (&word, &lim, quote))
        ;
      return quote == '\'' ? TOK_CHAR : TOK_STRING;
    }
  else
    return (unsigned char) c;
}

/* Skips whitespace and comments read from stdin.
   Returns nonzero if a newline was encountered, indicating that we're
   at the beginning of a line. */
static int
skipws (void)
{
  /* Classification of an input character. */
  enum class
    {
      CLS_WS = 0,               /* Whitespace. */
      CLS_BEG_CMT,              /* Slash-star beginning a comment. */
      CLS_END_CMT,              /* Star-slash ending a comment. */
      CLS_OTHER,                /* None of the above. */

      CLS_IN_CMT = 4            /* Combined with one of the above,
                                   indicates we're inside a comment. */
    };

  /* Either 0, if we're not inside a comment,
     or CLS_IN_CMT, if we are inside a comment. */
  enum class in_comment = 0;

  /* Have we encountered a newline outside a comment? */
  int beg_line = 0;
  
  for (;;)
    {
      int c;                    /* Input character. */
      enum class class;         /* Classification of `c'. */

      /* Get an input character and determine its classification. */
      c = getch ();
      switch (c)
        {
        case '\n':
          if (!in_comment)
            beg_line = 1;
          /* Fall through. */
          
        case ' ': case '\f': case '\r': case '\t': case '\v':
          class = CLS_WS;
          break;

        case '/':
          /* Outside a comment, slash-star begins a comment. */
          if (!in_comment)
            {
              c = getch ();
              if (c == '*')
                class = CLS_BEG_CMT;
              else
                {
                  ungetch (c);
                  c = '/';
                  class = CLS_OTHER;
                }
            }
          else
            class = CLS_OTHER;
          break;

        case '*':
          /* Inside a comment, star-slash ends the comment. */
          if (in_comment)
            {
              c = getch ();
              if (c == '/')
                class = CLS_END_CMT;
              else
                {
                  ungetch (c);
                  class = CLS_OTHER;
                }
            }
          else
            class = CLS_OTHER;
          break;

        default:
          /* Other characters. */
          if (c == EOF)
            return 0;
          class = CLS_OTHER;
        }

      /* Handle character `c' according to its classification
         and whether we're inside a comment. */
      switch (class | in_comment)
        {
        case CLS_WS:
        case CLS_WS | CLS_IN_CMT:
        case CLS_OTHER | CLS_IN_CMT:
          break;

        case CLS_BEG_CMT:
          in_comment = CLS_IN_CMT;
          break;

        case CLS_OTHER:
          ungetch (c);
          return beg_line;

        case CLS_END_CMT | CLS_IN_CMT:
          in_comment = 0;
          break;

        case CLS_BEG_CMT | CLS_IN_CMT:
        case CLS_END_CMT:
        default:
          printf ("can't happen\n");
          break;
        }
    }
}

/* Get a character inside a quoted string or character constant.
   QUOTE is ' for a character constant or " for a quoted string.
   *WORDP points to a string being constructed that has *LIMP bytes
   available. */
static int 
getstelem (char **wordp, int *limp, int quote)
{
  int c;

  /* Handle end-of-quote and EOF. */
  c = getch ();
  if (c == quote || c == EOF)
    return 0;

  /* Handle ordinary string characters. */
  if (c != '\\')
    {
      putch (wordp, limp, c);
      return 1;
    }

  /* We're in a \ escape sequence.
     Get the second character. */
  c = getch ();
  if (c == EOF)
    return 0;

  /* Handle simple single-character escapes. */
  {
    static const char escapes[] = {"''??\"\"\\\\a\ab\bf\fn\nr\rt\tv\v"};
    const char *cp = strchr (escapes, c);
    if (cp != NULL)
      {
        putch (wordp, limp, cp[1]);
        return 1;
      }
  }

  /* Handle hexadecimal and octal escapes.
     This also handles invalid escapes by default,
     doing nothing useful with them.
     That's okay because invalid escapes generate undefined behavior. */
  {
    unsigned char v = 0;

    if (c == 'x' || c == 'X')
      for (;;)
        {
          static const char hexits[] = "0123456789abcdef";
          const char *p;

          c = getch ();
          p = strchr (hexits, tolower ((unsigned char) c));
          if (p == NULL)
            break;
          v = v * 16 + (p - hexits);
        }
    else
      {
        int i;

        for (i = 0; i < 3; i++)
          {
            v = v * 8 + (c - '0');
            c = getch ();
            if (c < '0' || c > '7')
              break;
          }
      }
        
    putch (wordp, limp, v);
    ungetch (c);
  }
  
  return 1;
}

/* Capacity of putback buffer. */
#define BUFSIZE 100

/* Putback buffer. */
char buf[BUFSIZE];

/* Number of characters in putback buffer. */
int bufp = 0;

/* Retrieves and returns a character from stdin or from the putback
   buffer.
   Returns EOF if end of file is encountered. */
int 
getch (void)
{
  return bufp > 0 ? buf[--bufp] : getchar ();
}

/* Stuffs character C into the putback buffer.
   From the caller's perspective, fails silently if the putback buffer
   is full. */
void
ungetch (int c)
{
  if (c == EOF)
    return;
  
  if (bufp >= BUFSIZE)
    printf ("ungetch: too many characters\n");
  else
    buf[bufp++] = c;
}

/* Stuffs character C into buffer *WORDP, which has *LIMP bytes
   available.
   Advances *WORDP and reduces *LIMP as appropriate.
   Drops the character on the floor if it would overflow the buffer.
   Ensures that *WORDP is null terminated if possible. */
static void 
putch (char **wordp, int *limp, int c)
{
  if (*limp > 1)
    {
      *(*wordp)++ = c;
      (*limp)--;
    }
  if (*limp > 0)
    **wordp = '\0';
}

/* 
   Local variables:
   compile-command: "checkergcc -W -Wall -ansi -pedantic knr61.c -o knr61"
   End: 
*/

