/*
 *  Wrapper library for 'indent' that formats C like Clifford likes it..
 *
 *  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
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


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if WIN32

int wrindent(const char *input, char **output, char **error)
{
	*output = NULL;
	*error = strdup("Indent Wrapper is not implemented for Win32 yet.\n");
	return 1;
}

#else

static char *file_to_charp(FILE * f)
{
	int size = 4096, pos = 0;
	char *buffer = malloc(size);
	while (1) {
		if (size - pos < 1024)
			buffer = realloc(buffer, size *= 2);
		int rc = fread(buffer + pos, 1, size - pos - 1, f);
		if (rc < 0)
			break;
		pos += rc;
		if (rc < size - pos - 1)
			break;
	}
	buffer[pos] = 0;
	return buffer;
}

int is_big_block(const char *str)
{
	int depth = 1;
	
	while (str[0] && depth)
	{
		if (str[0] == '/' && str[1] == '*') {
			while (str[0] && str[1] && (str[0] != '*' || str[1] != '/'))
				str++;
			if (str[0])
				str++;
			if (str[0])
				str++;
			continue;
		}
		
		if (str[0] == '/' && str[1] == '/') {
			while (str[0] && str[0] != '\n')
				str++;
			continue;
		}
		
		if (str[0] == '"' || str[0] == '\'') {
			char ch = str[0];
			str++;
			while (str[0] && str[0] != ch) {
				if (str[0] == '\\' && str[1])
					str++;
				str++;
			}
			if (str[0])
				str++;
			continue;
		}
		
		if (str[0] == '{') {
			str++;
			depth++;
			continue;
		}
		if (str[0] == '}') {
			str++;
			depth--;
			continue;
		}
		
		if (str[0] == '\n' && str[1] == '\n')
			return 1;
		if ((str[0] == '\n' || str[0] == '\t') && !strncmp(str + 1, "case ", 5))
			return 1;
			
		str++;
	}
	
	return 0;
}

#define C(_ch) do { \
	if (output_pos == output_size) \
		*output = realloc(*output, output_size *= 2); \
	(*output)[output_pos++] = _ch; \
} while (0)

int wrindent(const char *input, char **output, char **error)
{
	const char *tempdir = "/tmp";
	
	char tempfn_in[128];
	snprintf(tempfn_in, 128, "%s/cedit_indent_tempfile_in_XXXXXX", tempdir);
	close(mkstemp(tempfn_in));
	
	char tempfn_out[128];
	snprintf(tempfn_out, 128, "%s/cedit_indent_tempfile_out_XXXXXX", tempdir);
	close(mkstemp(tempfn_out));
	
	FILE *f = fopen(tempfn_in, "w");
	fwrite(input, strlen(input), 1, f);
	fclose(f);
	
	char command[1024];
	snprintf(command, 1024, "indent -kr -i8 -l0 '%s' -o '%s' 2>&1", tempfn_in, tempfn_out);
	
	f = popen(command, "r");
	*error = file_to_charp(f);
	int rc = pclose(f);
	
	f = fopen(tempfn_out, "r");
	char *temp = file_to_charp(f);
	fclose(f);
	
	remove(tempfn_in);
	remove(tempfn_out);
	
	int output_pos = 0;
	int output_size = strlen(temp);
	*output = malloc(output_size);
	
	int indent_start = 0;
	int indent_len = 0;
	
	int new_indent_start = -1;
	int new_indent_len = 0;
	
	int i, j;
	for (i = 0; temp[i];)
	{
		if (temp[i] == '/' && temp[i + 1] == '*') {
			while (temp[i] && temp[i + 1] && (temp[i] != '*' || temp[i + 1] != '/'))
				C(temp[i++]);
			if (temp[i])
				C(temp[i++]);
			if (temp[i])
				C(temp[i++]);
			continue;
		}
		
		if (temp[i] == '/' && temp[i + 1] == '/') {
			while (temp[i] && temp[i] != '\n')
				C(temp[i++]);
			continue;
		}
		
		if (temp[i] == '"' || temp[i] == '\'') {
			char ch = temp[i];
			C(temp[i++]);
			while (temp[i] && temp[i] != ch) {
				if (temp[i] == '\\' && temp[i + 1])
					C(temp[i++]);
				C(temp[i++]);
			}
			if (temp[i])
				C(temp[i++]);
			continue;
		}
		
		if (temp[i] == ' ' && temp[i + 1] == '{' && is_big_block(temp + i + 2)) {
			C('\n');
			for (j = 0; j < indent_len; j++)
				C(temp[indent_start + j]);
			i++;
			continue;
		}
		
		if (i > 0 && temp[i] == '\n' && temp[i - 1] == '\n') {
			for (j = 0; j < indent_len; j++)
				C(temp[indent_start + j]);
		}
		
		if (temp[i] == '\n') {
			new_indent_start = i + 1;
			new_indent_len = 0;
		} else if (temp[i] == '\t' && new_indent_start >= 0) {
			new_indent_len++;
		} else if (new_indent_start >= 0) {
			indent_start = new_indent_start;
			indent_len = new_indent_len;
			new_indent_start = -1;
		}
		
		C(temp[i++]);
	}
	
	free(temp);
	*output = realloc(*output, output_pos + 1);
	(*output)[output_pos] = 0;
	
	if ((*error)[0] == 0) {
		free(*error);
		*error = NULL;
	}
	
	if ((*output)[0] == 0) {
		free(*output);
		*output = NULL;
	}
	
	return rc;
}

#ifdef WRINDENT_MAIN
int main()
{
	char *input = NULL;
	char *output, *error;
	int rc;
	
	{
		size_t buflen = 0;
		FILE *buf = open_memstream(&input, &buflen);
		while ((rc = fgetc(stdin)) >= 0)
			fputc(rc, buf);
		fclose(buf);
	}
	
	rc = wrindent(input, &output, &error);
	
	if (error) {
		fprintf(stderr, "%s", error);
		rc = rc ? : 1;
	} else if (rc) {
		fprintf(stderr, "Unkown error!\n");
	} else {
		fprintf(stdout, "%s", output);
	}
	
	free(input);
	free(output);
	free(error);
	
	return rc;
}
#endif

#endif
