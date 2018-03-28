/*
 *  clfindent.c - An indent for cliffords C coding style
 *
 *  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#define _GNU_SOURCE 1
#undef DEBUG_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef DEBUG_FUNCTIONS
#  include <stdarg.h>
#endif

struct malloc_list {
	struct malloc_list *next;
};

static struct malloc_list *ml;
static struct token *tl;
static struct token **tl_last_ptr;
static struct token *tl_set_prev;

struct out_listent_s {
	struct malloc_list ml;
	const char *str;
	int str_len;
	struct out_listent_s *next;
};

static struct out_listent_s *out_list;
static struct out_listent_s **out_list_nextp;
static int out_len;

struct token {
	struct malloc_list ml;
	struct token *prev;
	struct token *next;
	int blank_left;
	int blank_right;
	int newline_left;
	int newline_right;
	int stick_to_id;
	int is_id;
	int is_blank_line;
	int maybe_end_of_statement;
	int not_a_nested_statement;
	int command_level_delta;
	int is_ptr_operator;
	int is_pp_statement;
	int next_may_be_ptr_operator;
	int next_newline_ok;
	int next_newline_boost;
	int is_block_open;
	int is_block_close;
	int is_label;
	int is_semikolon;
	int is_switch_label;
	int is_else;
	int is_string;
	const char *str;
#ifdef DEBUG_FUNCTIONS
	const char *debug;
#endif
	int rel_cursor_pos;
};

static const char *long_operators[] = {
	"++", "--", "+=", "-=", "*=", "/=", "%=", "^=", "&=", "|=",
	"&&", "||", "->", "==", "!=", "<=", ">=", "<<", ">>", NULL
};

static const char *sticky_operators[] = {
	"->", "++", "--", "!", "~", NULL
};

static const char *keyword_with_blank[] = {
	"if", "while", "for", "switch", "return"
};

static void *my_malloc(int size)
{
	struct malloc_list *n = malloc(size);
	memset(n, 0, size);
	n->next = ml;
	ml = n;
	return n;
}

static char *my_strndup(const char *data, int len)
{
	char *p = my_malloc(sizeof(struct malloc_list) + len + 1) + sizeof(struct malloc_list);
	memcpy(p, data, len);
	p[len] = 0;
	return p;
}

static struct token *new_token(const char *str, int cursor_pos, int pos)
{
	struct token *t = my_malloc(sizeof(struct token));
	*tl_last_ptr = t;
	t->prev = tl_set_prev;
	tl_set_prev = t;
	tl_last_ptr = &t->next;
	t->str = str;
	t->rel_cursor_pos = cursor_pos - pos;
	return t;
}

#ifdef DEBUG_FUNCTIONS
__attribute__((unused))
static void token_debugf(struct token *t, const char *fmt, ...)
{
	char *buf;
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vasprintf(&buf, fmt, ap);
	va_end(ap);

	if (t->debug) {
		const char *old_dbg = t->debug;
		t->debug = NULL;
		token_debugf(t, "%s%s", old_dbg, buf);
	} else {
		t->debug = my_strndup(buf, n);
	}
	free(buf);
}
#endif

static void my_free()
{
	struct malloc_list *n = ml;
	while (n) {
		struct malloc_list *x = n->next;
		free(n);
		n = x;
	}
	ml = 0;
	tl = 0;
	tl_last_ptr = &tl;
}

static int my_alphanum(int ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_' || ch == '.';
}

static void parse(const char *input, int cursor_pos)
{
	int switch_label_active = 1;
	int attr_flags[100] = { };
	int expr_level = 0;
	int cmd_level = 0;
	int after_eq = 0;
	int pos = 0;

	while (input[pos])
	{
		/* whitespaces */
		if (input[pos] == ' ' || input[pos] == '\n' || input[pos] == '\t') {
			int nl_count = 0;
			while (input[pos] == ' ' || input[pos] == '\n' || input[pos] == '\t') {
				if (input[pos] == '\n' && nl_count == 0 && tl_set_prev && tl_set_prev->next_newline_ok)
					tl_set_prev->newline_right = 1;
				if (input[pos] == '\n' && nl_count++ > 0) {
					struct token *t = new_token("", cursor_pos, pos);
					t->newline_right = 1;
					t->is_blank_line = 1;
				}
				pos++;
			}
			continue;
		}

		/* preprocessor statement */
		if (input[pos] == '#') {
			int len = 1;
			while (input[pos + len] && input[pos + len] != '\n')
				len++;
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			t->rel_cursor_pos = cursor_pos - pos;
			t->is_pp_statement = 1;
			t->newline_left = 1;
			t->newline_right = 1;
			pos += len;
			continue;
		}

		/* identifier or label */
		if (my_alphanum(input[pos])) {
			int len = 1;
			int is_label = 0;
			if (after_eq || expr_level) {
				while (my_alphanum(input[pos + len]))
					len++;
			} else {
				while ((my_alphanum(input[pos + len]) || input[pos + len] == ':') && !is_label) {
					if (input[pos + len] == ':' && len == strlen("default") && !strncmp(input + pos, "default", len))
						break;
					if (input[pos + len] == ':')
						is_label = 1;
					len++;
				}
				if (is_label && input[pos + len] == ';')
					len++;
			}
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			if (!strcmp(t->str, "else")) {
				if (t->prev && t->prev->command_level_delta < 0) {
					t->prev->newline_right = 0;
					t->prev->blank_right = 1;
				}
				t->maybe_end_of_statement = 1;
				t->is_else = 1;
			}
			if (!strcmp(t->str, "if") && t->prev && t->prev->is_else) {
				t->prev->maybe_end_of_statement = 0;
			}
			if (!strcmp(t->str, "__attribute__") && expr_level < 100) {
				attr_flags[expr_level] = 1;
			}
			if (!strcmp(t->str, "case") || !strcmp(t->str, "default")) {
				switch_label_active = 1;
				t->is_switch_label = 1;
				if (!strcmp(t->str, "case"))
					t->blank_right = 1;
			}
			if (is_label) {
				t->is_label = 1;
				t->newline_right = 1;
			} else {
				t->is_id = 1;
				if (t->prev && (t->prev->is_id || t->prev->is_string))
					t->blank_left = 1;
			}
			int i;
			for (i = 0; keyword_with_blank[i]; i++)
				if (!strcmp(keyword_with_blank[i], t->str)) {
					t->blank_right = 1;
					break;
				}
			pos += len;
			continue;
		}

		/* string */
		if (input[pos] == '"' || input[pos] == '\'') {
			int len = 1;
			while (input[pos + len] && input[pos + len] != input[pos]) {
				if (input[pos + len] == '\\' && input[pos + len + 1])
					len++;
				len++;
			}
			if (input[pos + len])
				len++;
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			if (t->prev && (t->prev->is_id || t->prev->is_string))
				t->blank_left = 1;
			t->is_string = 1;
			pos += len;
			continue;
		}

		/* comment (double-slash) */
		if (input[pos] == '/' && input[pos + 1] == '/') {
			int len = 1;
			while (input[pos + len] && input[pos + len] != '\n')
				len++;
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			t->newline_right = 1;
			pos += len;
			continue;
		}

		/* comment (slash-star) */
		if (input[pos] == '/' && input[pos + 1] == '*') {
			int len = 2;
			while (input[pos + len] && input[pos + len + 1] && (input[pos + len] != '*' || input[pos + len + 1] != '/'))
				len++;
			if (input[pos + len] == '*' && input[pos + len + 1] == '/')
				len += 2;
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			t->newline_right = 1;
			pos += len;
			continue;
		}

		/* end of switch label */
		if (input[pos] == ':' && switch_label_active) {
			int len = 1;
			if (input[pos + 1] == ';')
				len++;
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			t->newline_right = 1;
			switch_label_active = 0;
			pos++;
			continue;
		}

		/* end of command or argument */
		if (input[pos] == ';' || input[pos] == ',') {
			struct token *t = new_token(my_strndup(input + pos, 1), cursor_pos, pos);
			if (!expr_level && input[pos] == ';') {
				if (t->prev && t->prev->command_level_delta == -1)
					t->prev->newline_right = 0;
				t->newline_right = 1;
				t->not_a_nested_statement = 1;
				t->is_semikolon = 1;
			} else {
				t->blank_right = 1;
				if (expr_level)
					t->next_newline_boost = 1;
				t->next_newline_ok = 1;
			}
			t->next_may_be_ptr_operator = 1;
			after_eq = 0;
			pos++;
			continue;
		}

		/* command block */
		if (input[pos] == '{' || input[pos] == '}') {
			struct token *t = new_token(my_strndup(input + pos, 1), cursor_pos, pos);
			if (t->prev && strcmp(t->prev->str, "=") && (strcmp(t->prev->str, "{") || input[pos] == '{'))
				t->newline_left = 1;
			if (input[pos] == '{') {
				int i;
				for (i = 1; input[pos + i] == ' '; i++) { }
				if (input[pos + i] == '}')
					t->blank_right = 1;
				else
					t->newline_right = 1;
			} else {
				t->newline_right = 1;
			}
			t->not_a_nested_statement = 1;
			t->command_level_delta = input[pos] == '{' ? +1:
			-1;
			cmd_level += t->command_level_delta;
			after_eq = 0;
			pos++;
			continue;
		}

		/* expr block start */
		if (input[pos] == '(' || input[pos] == '[') {
			struct token *t = new_token(my_strndup(input + pos, 1), cursor_pos, pos);
			t->next_may_be_ptr_operator = 1;
			t->not_a_nested_statement = 1;
			t->is_block_open = 1;
			expr_level++;
			pos++;
			continue;
		}

		/* expr block end */
		if (input[pos] == ')' || input[pos] == ']') {
			struct token *t = new_token(my_strndup(input + pos, 1), cursor_pos, pos);
			expr_level--;
			if (expr_level < 100 && attr_flags[expr_level]) {
				attr_flags[expr_level] = 0;
				if (input[pos + 1] == '\n')
					t->newline_right = 1;
			} else if (!after_eq && !expr_level && input[pos] == ')') {
				t->maybe_end_of_statement = 1;
			}
			if (t->prev)
				t->prev->blank_right = 0;
			t->is_block_close = 1;
			pos++;
			continue;
		}

		/* everything else is an operator */
		{
			int i;
			int len = 1;
			int is_sticky = 0;
			for (i = 0; long_operators[i]; i++)
				if (!strncmp(input + pos, long_operators[i], strlen(long_operators[i]))) {
					len = strlen(long_operators[i]);
					break;
				}
			struct token *t = new_token(my_strndup(input + pos, len), cursor_pos, pos);
			t->next_newline_ok = 1;
			t->next_newline_boost = 1;
			t->next_may_be_ptr_operator = 1;
			t->not_a_nested_statement = 1;
			for (i = 0; sticky_operators[i]; i++)
				if (!strcmp(sticky_operators[i], t->str)) {
					is_sticky = 1;
					break;
				}
			if (!is_sticky) {
				if (!t->prev || !t->prev->is_block_open)
					t->blank_left = 1;
				t->blank_right = 1;
			}

			if (t->str[0] == '=' && !t->str[1]) {
				after_eq = 1;
			}
			if ((t->str[0] == '+' || t->str[0] == '-') && !t->str[1]) {
				if (t->prev && !t->prev->is_block_close && !t->prev->is_id)
					t->blank_right = 0;
			}
			if ((t->str[0] == '*' || t->str[0] == '&') && !t->str[1]) {
				if (t->prev && t->prev->maybe_end_of_statement) {
					t->not_a_nested_statement = 0;
					goto is_ptr_operator;
				}
				if (t->prev && t->prev->next_may_be_ptr_operator)
					goto is_ptr_operator;
				if (!after_eq && (!cmd_level || !expr_level))
					goto is_ptr_operator;
				if (0) {
is_ptr_operator:
					t->is_ptr_operator = 1;
					if (t->prev && t->prev->is_ptr_operator)
						t->blank_left = 0;
					t->blank_right = 0;
				}
			}
			pos += len;
			continue;
		}
	}
	new_token(NULL, cursor_pos, pos);
}

static void mangle()
{
	int cmd_level = 0;
	int boosted_indent = 0;
	struct token *t = tl;
	while (t && t->str) {
		if (t->command_level_delta > 0) {
			int big_block_mode = boosted_indent;
			int boosted_indent2 = boosted_indent;
			cmd_level += t->command_level_delta;
			int temp_cmd_level = cmd_level;
			struct token *t2 = t->next;
			while (t2 && t2->str && temp_cmd_level >= cmd_level) {
				if (t2->command_level_delta > 0 && boosted_indent2)
					big_block_mode = 1;
				if (t2->newline_right)
					boosted_indent2 = t2->next_newline_boost;
				if (t2->newline_left)
					boosted_indent2 = t2->prev && t2->prev->next_newline_boost;
				if (t2->is_blank_line || t2->is_switch_label)
					big_block_mode = 1;
				temp_cmd_level += t2->command_level_delta;
				t2 = t2->next;
			}
			if ((cmd_level > 1 || (t->prev && !t->prev->is_block_close)) && !big_block_mode) {
				t->newline_left = 0;
				t->blank_left = 1;
			} else if (t2 && t2->str && t2->is_else) {
				t2->newline_left = 1;
			}
		}
		if (t->newline_right)
			boosted_indent = t->next_newline_boost;
		if (t->newline_left)
			boosted_indent = t->prev && t->prev->next_newline_boost;
		if (t->next->command_level_delta < 0)
			cmd_level += t->next->command_level_delta;
		t = t->next;
	}
}

static void out_puts(const char *str)
{
	struct out_listent_s *e = my_malloc(sizeof(struct out_listent_s));
	e->str = str;
	e->str_len = strlen(str);
	out_len += e->str_len;
	*out_list_nextp = e;
	out_list_nextp = &e->next;
}

#ifdef DEBUG_FUNCTIONS
static void out_debugf(const char *fmt, ...)
{
	char *buf;
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vasprintf(&buf, fmt, ap);
	va_end(ap);

	out_puts(my_strndup(buf, n));
	free(buf);
}
#endif

static void dump(int blank_indent, int *pos)
{
	int i;
	int indent_stack[100];
	int indent_adhoc[100];
	int indent_idx = 0;
	struct token *t = tl;
	while (t && t->str) {
		if (pos && t->rel_cursor_pos >= 0)
			*pos = out_len + t->rel_cursor_pos;
#ifdef DEBUG_FUNCTIONS
		if (t->debug) {
			out_debugf("$%s: %s$", t->debug, t->str);
		} else
#endif
			out_puts(t->str);
		if (t->command_level_delta > 0 && indent_idx < 90) {
			indent_stack[indent_idx] = indent_idx > 0 ? indent_stack[indent_idx - 1] + 1 : 1;
			indent_adhoc[indent_idx] = 0;
			indent_idx++;
		}
		if (t->command_level_delta < 0) {
			if (indent_idx > 0)
				indent_idx--;
			if (indent_idx > 0 && indent_adhoc[indent_idx - 1])
				indent_idx--;
		}
		if (t->next->command_level_delta < 0) {
			while (indent_idx > 0 && indent_adhoc[indent_idx - 1])
				indent_idx--;
			if (indent_idx > 0)
				indent_stack[indent_idx - 1]--;
		}
		if (t->is_semikolon && indent_idx > 0 && indent_adhoc[indent_idx - 1]) {
			indent_idx--;
		}
		if (t->maybe_end_of_statement && !t->next->not_a_nested_statement) {
			if (indent_idx <= 0 || !indent_adhoc[indent_idx - 1]) {
				indent_stack[indent_idx] = indent_idx > 0 ? indent_stack[indent_idx - 1] : 0;
				indent_adhoc[indent_idx] = 1;
				indent_idx++;
			}
			indent_stack[indent_idx - 1]++;
			goto generate_newline;
		} else if (t->newline_right || t->next->newline_left) {
			goto generate_newline;
		} else if (t->blank_right || t->next->blank_left) {
			out_puts(" ");
		} else if (t->is_id && t->next->is_id) {
			out_puts(" ");
		}
		if (0) {
generate_newline:
			out_puts("\n");
			if (blank_indent || !t->next->is_blank_line) {
				if (t->next_newline_boost)
					out_puts("\t\t");
				if (!t->next->is_pp_statement && !t->next->is_label && indent_idx > 0) {
					int next_is_switch_label = t->next && t->next->is_switch_label;
					for (i = next_is_switch_label ? 1 : 0; i < indent_stack[indent_idx - 1]; i++)
						out_puts("\t");
				}
			}
		}
		t = t->next;
	}
}

char *clfindent(const char *code, int blank_indent, int *cursor_pos)
{
	char *output, *p;
	struct out_listent_s *e;

	ml = NULL;
	tl = NULL;
	tl_last_ptr = &tl;
	tl_set_prev = NULL;

	out_list = NULL;
	out_list_nextp = &out_list;
	out_len = 0;

	parse(code, cursor_pos ? *cursor_pos : -1);
	mangle();
	dump(blank_indent, cursor_pos);

	if (cursor_pos && *cursor_pos > out_len)
		*cursor_pos = out_len;

	output = malloc(out_len + 1);
	for (e = out_list, p = output; e; e = e->next) {
		memcpy(p, e->str, e->str_len);
		p += e->str_len;
	}
	*p = 0;

	my_free();
	return output;
}

int worker(int do_inplace, int blank_indent, int cursor_pos, const char *filename)
{
	char *buf_ptr = NULL;
	size_t buf_size;
	FILE *buf, *f;
	int ret = 0;
	int ch;

	if (filename) {
		f = fopen(filename, "r");
		if (!f) {
			fprintf(stderr, "Can't open `%s' for reading.\n", filename);
			return 1;
		}
	} else {
		f = stdin;
	}

	buf = open_memstream(&buf_ptr, &buf_size);
	while ((ch = fgetc(f)) >= 0)fputc(ch, buf);
	fclose(buf);

	if (filename) {
		char *new_filename;
		fclose(f);
		if (do_inplace) {
			new_filename = (char *)filename;
		} else {
			asprintf(&new_filename, "%s~", filename);
		}
		f = fopen(new_filename, "w");
		if (!f) {
			fprintf(stderr, "Can't open `%s' for writing .\n", filename);
			ret = 1;
		}
		if (!do_inplace)
			free(new_filename);
	} else {
		f = stdout;
	}

	if (!ret) {
		char *output = clfindent(buf_ptr, blank_indent, cursor_pos >= 0 ? &cursor_pos : NULL);
		if (cursor_pos >= 0) {
			printf("%d\n", cursor_pos);
		}
		if (cursor_pos < 0 || filename) {
			fprintf(f, "%s", output);
		}
		free(output);
	}

	if (filename)
		fclose(f);

	free(buf_ptr);
	return ret;
}

void help(const char *command)
{
	fprintf(stderr, "\n");
	fprintf(stderr, "Usage: %s [-i] [-b] [-c N] [filename [..]]\n", command);
	fprintf(stderr, "\n");
	fprintf(stderr, "  -i     operate in-place\n");
	fprintf(stderr, "         (per default the new text is written to a ...~ file)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -b     do also indent blank lines\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "  -c N   N is a cursor position as byte-offset. The new cursor\n");
	fprintf(stderr, "         position in the indented file is written to stdout.\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char **argv)
{
	int opt;
	int do_inplace = 0;
	int blank_indent = 0;
	int cursor_pos = -1;
	int errors = 0;

	while ((opt = getopt(argc, argv, "ibc:")) != -1)
	{
		switch (opt)
		{
		case 'i':
			do_inplace = 1;
			break;
		case 'b':
			blank_indent = 1;
			break;
		case 'c':
			cursor_pos = atoi(optarg);
			break;
		default:
			help(argv[0]);
		}
	}

	if (optind >= argc)
		return worker(do_inplace, blank_indent, cursor_pos, NULL);

	while (optind < argc)
		errors += worker(do_inplace, blank_indent, cursor_pos, argv[optind++]);

	return errors > 0 ? 1 : 0;
}

