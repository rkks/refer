#ifndef _STABS_H_
# define _STABS_H_

typedef struct	 s_StabH
{
  long		n_strx;
  unsigned char	n_type;
  char		n_other;
  short		n_desc;
  unsigned	n_value;
}		t_StabH;

typedef struct	s_type
{
  char		*t_name;
  int		t_fnum;
  int		t_tnum;
  char		t_ptr;
  int		t_size;
  char		t_flags;
# define TYPEFLAGS_UNDEF	0x00
# define TYPEFLAGS_STRUCT	0x01
# define TYPEFLAGS_ENUM		0x02
  struct s_type	*t_parenttype;
  struct s_type	*t_next;
}		t_type;

typedef struct	s_line
{
  int		l_line;
  unsigned int	l_addr;
}		t_line;

typedef struct	s_argument
{
  char		*a_name;
  int		a_type[2];
}		t_argument;

typedef struct	s_function
{
  char		*f_name;
  unsigned int	f_addr;
  int		f_rettype[2];
  t_argument	*f_argstype;
  int		f_nargs;
}		t_function;

typedef struct	s_alias
{
  int		a_fromidx;
  int		a_toidx;
  int		a_fileidx;
}		t_alias;

typedef struct	s_file
{
  char		*f_name;
  t_line	*f_lines;
  int		f_nlines;
  t_function	*f_functions;
  int		f_nfunctions;
  t_type	*f_types;
  t_alias	*f_aliases;
  int		f_naliases;
}		t_file;

typedef struct	s_header
{
  int		h_idx;
  int		h_instance;
  char		*h_path;
  char		*h_from;
}		t_header;

typedef struct	s_parse
{
  char		*p_origname;
  char		*p_name;
  char		*p_origtag;
  char		*p_tag;
  char		p_descriptor;
  char		*p_orignext;
  char		*p_next;
}		t_parse;

typedef struct	s_parsecontext
{
  char		*p_orig;
  char		*p_buf;
}		t_parsecontext;

typedef struct		s_stackelem
{
  char			*s_buf;
  struct s_stackelem	*s_next;
}			t_stackelem;

typedef struct	s_basetype
{
  char		*b_name;
  int		b_size;
}		t_basetype;

# define CONTEXT_NEW(c, v)	((c)->p_buf = (c)->p_orig = strdup(v))
# define CONTEXT_FREE(c)	(free((c)->p_orig))
# define CONTEXT_BUF(c)		((c)->p_buf)

# include <elf.h>
# include <stab.h>

#endif /* !_STABS_H_ */
