/* Lew Pitcher <lpitcher@yesic.com> */

/*/
** derem - remove C comments
**
** (attempt to solve K&R Exercise 1-22)
**
** As I only have v1 copy of K&R, I cannot
** be sure what is covered in K&R ANSI chapter 1.
** So, I restrict myself to the components covered
** in K&R v1 chapter 1, but modified for requisite ANSI
** features (int main() and return value).
**
** Components covered in v1 K&R chapter 1 include:
**  while (), for (), if () else
**  getchar(), putchar(), EOF
**  character constants, character escapes
**  strings
**  array subscripting
**
** Not directly covered are
**  string subscripting ( "/*"[0] )
**  initializers ( int state = PROGRAM; )
**/

/*/*/

#include <stdio.h>

#define	PROGRAM		0
#define	BEGIN_COMMENT	1
#define	COMMENT		2
#define	END_COMMENT	3
#define	QUOTE		4

int main(void)
{
	int this_char, quote_char;
	int state;

	state = PROGRAM;

	while ((this_char = getchar()) != EOF)
	{
		if (state == PROGRAM)
		{
			if (this_char == '/')
				state = BEGIN_COMMENT;
			else if ((this_char == '"') || (this_char == '\''))
			{
				state = QUOTE;
				putchar(quote_char = this_char);
			}
			else	putchar(this_char);
		}
		else if (state == BEGIN_COMMENT)
		{
			if (this_char == '*')
				state = COMMENT;
			else
			{
				putchar('/'); /* for the '/' of the comment */
				if (this_char != '/')
				{
					state = PROGRAM;
					putchar(this_char);
				}
				else	state = COMMENT;	/* stuttered */
			}
		}
		else if (state == QUOTE)
		{
			putchar(this_char);
			if (this_char == '\\')
				putchar(getchar());	/* escaped character */
			else if (this_char == quote_char)
				state = PROGRAM;
		}
		else if (state == COMMENT)
		{
			if (this_char == '*')
				state = END_COMMENT;
		}
		else if (state == END_COMMENT)
		{
			if (this_char == '/')
				state = PROGRAM;
			else if (this_char != '*')	/* stuttered */
				state = COMMENT;
		}
	}

	return 0;
}

