/*
 *  Simple OpenGL demo program with shaders
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
 *  Compile:
 *  gcc -o gldemo -lSDL -lGLEW -lm gldemo.c
 */

// this must be included before any OpenGL headers
#include <GL/glew.h>

#include <SDL/SDL.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

/**** BEGIN: http://svn.clifford.at/tools/trunk/examples/check.h ****/

// This is to not confuse the VIM syntax highlighting
#define CHECK_VAL_OPEN (
#define CHECK_VAL_CLOSE )

#define CHECK(result, check)                                           \
   CHECK_VAL_OPEN{                                                     \
     typeof(result) _R = (result);                                     \
     if (!(_R check)) {                                                \
       fprintf(stderr, "Error from '%s' (%d %s) in %s:%d.\n",          \
                       #result, (int)_R, #check, __FILE__, __LINE__);  \
       fprintf(stderr, "ERRNO(%d): %s\n", errno, strerror(errno));     \
       abort();                                                        \
     }                                                                 \
     _R;                                                               \
   }CHECK_VAL_CLOSE

/**** END: http://svn.clifford.at/tools/trunk/examples/check.h ****/

SDL_Surface *screen;

GLuint prog = 0;
double theta = 1, phi = 1;

void checkGlError(const char *file, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		fprintf(stderr, "OpenGL Error at %s:%d: %s\n", file, line, gluErrorString(err));
	}
}

void checkGlShader(GLuint shader, const char *file, int line)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Shader Compile Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
	} else {
		int loglen;
		char logbuffer[1000];
		glGetShaderInfoLog(shader, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Shader Compile OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
	}
}

void checkGlProgram(GLuint prog, const char *file, int line)
{
	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		fprintf(stderr, "OpenGL Program Linker Error at %s:%d:\n%.*s", file, line, loglen, logbuffer);
	} else {
		int loglen;
		char logbuffer[1000];
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Link OK at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
		glValidateProgram(prog);
		glGetProgramInfoLog(prog, sizeof(logbuffer), &loglen, logbuffer);
		if (loglen > 0) {
			fprintf(stderr, "OpenGL Program Validation results at %s:%d:\n%.*s", file, line, loglen, logbuffer);
		}
	}
}

#define CHECKGL(...)                             \
   do {                                          \
     checkGlError("(pre) " __FILE__, __LINE__);  \
     __VA_ARGS__;                                \
     checkGlError(__FILE__, __LINE__);           \
   } while (0)

#define CHECKGL_V(result)                        \
   CHECK_VAL_OPEN{                               \
     checkGlError("(pre) " __FILE__, __LINE__);  \
     typeof(result) _R = result;                 \
     checkGlError(__FILE__, __LINE__);           \
     _R;                                         \
   }CHECK_VAL_CLOSE

void draw()
{
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
	glDepthRange(-100, 100);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, +1.0, -1.0, +1.0, +1.5, +20.0);
	gluLookAt(5.0 * cos(theta) * cos(phi), 5.0 * sin(theta), 5.0 * cos(theta) * sin(phi), 0.0, 0.0, 0.0,
			5.0 * cos(theta+1) * cos(phi), 5.0 * sin(theta+1), 5.0 * cos(theta+1) * sin(phi));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_LINE_STRIP); glVertex3d(-5,  0,  0); glVertex3d(+5,  0,  0); glEnd();
	glBegin(GL_LINE_STRIP); glVertex3d( 0, -5,  0); glVertex3d( 0, +5,  0); glEnd();
	glBegin(GL_LINE_STRIP); glVertex3d( 0,  0, -5); glVertex3d( 0,  0, +5); glEnd();

	glUseProgram(prog);
	GLint yloc = glGetAttribLocation(prog, "y");
	glBegin(GL_POLYGON);
	glVertexAttrib1d(yloc, 0.0); glColor3d(1.0, 0.0, 0.0); glVertex3d(+2, -1, +0);
	glVertexAttrib1d(yloc, 0.0); glColor3d(0.0, 0.0, 1.0); glVertex3d(+0, -1, +2);
	glVertexAttrib1d(yloc, 1.0); glColor3d(0.0, 1.0, 0.0); glVertex3d(+0, +1, +0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertexAttrib1d(yloc, 0.0); glColor3d(0.0, 0.0, 1.0); glVertex3d(+0, -1, +2);
	glVertexAttrib1d(yloc, 0.0); glColor3d(1.0, 0.0, 0.0); glVertex3d(-2, -1, +0);
	glVertexAttrib1d(yloc, 1.0); glColor3d(0.0, 1.0, 0.0); glVertex3d(+0, +1, +0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertexAttrib1d(yloc, 0.0); glColor3d(1.0, 0.0, 0.0); glVertex3d(-2, -1, +0);
	glVertexAttrib1d(yloc, 0.0); glColor3d(0.0, 0.0, 1.0); glVertex3d(+0, -1, -2);
	glVertexAttrib1d(yloc, 1.0); glColor3d(0.0, 1.0, 0.0); glVertex3d(+0, +1, +0);
	glEnd();
	glBegin(GL_POLYGON);
	glVertexAttrib1d(yloc, 0.0); glColor3d(0.0, 0.0, 1.0); glVertex3d(+0, -1, -2);
	glVertexAttrib1d(yloc, 0.0); glColor3d(1.0, 0.0, 0.0); glVertex3d(+2, -1, +0);
	glVertexAttrib1d(yloc, 1.0); glColor3d(0.0, 1.0, 0.0); glVertex3d(+0, +1, +0);
	glEnd();

	glUseProgram(0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP); glVertex3d(+2, -1, +0); glVertex3d(+0, -1, +2); glVertex3d(+0, +1, +0); glEnd();
	glBegin(GL_LINE_LOOP); glVertex3d(+0, -1, +2); glVertex3d(-2, -1, +0); glVertex3d(+0, +1, +0); glEnd();
	glBegin(GL_LINE_LOOP); glVertex3d(-2, -1, +0); glVertex3d(+0, -1, -2); glVertex3d(+0, +1, +0); glEnd();
	glBegin(GL_LINE_LOOP); glVertex3d(+0, -1, -2); glVertex3d(+2, -1, +0); glVertex3d(+0, +1, +0); glEnd();

	CHECKGL();
	SDL_GL_SwapBuffers();
}

int main(int argc, char **argv)
{
	CHECK(SDL_Init(SDL_INIT_VIDEO), >= 0);
	SDL_WM_SetCaption("OpenGL Demo", "OpenGL Demo");
	atexit(SDL_Quit);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = CHECK(SDL_SetVideoMode(640, 480, 0, SDL_OPENGL), != NULL);

	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(glew_err));
		return 1;
	}

	if (!glewIsSupported("GL_VERSION_2_0")) {
		fprintf(stderr, "Accoring to GLEW there is no OpenGL 2.0 support!\n");
		return 1;
	}

	char *vs_source =
		"attribute float y;\n"
		"varying float vy;\n"
		"void main() {\n"
		"	gl_FrontColor = gl_Color;\n"
		"	gl_Position = ftransform();\n"
		"	vy = y;\n"
		"}\n";
	char *fs_source =
		"#extension GL_EXT_gpu_shader4 : enable\n"
		"varying float vy;\n"
		"void main() {\n"
		"	if (int(vy*15.0) % 2 == 1)\n"
		"		discard;\n"
		"	gl_FragColor = gl_Color;\n"
		"}\n";

	prog = glCreateProgram();

	GLuint vs = CHECKGL_V(glCreateShader(GL_VERTEX_SHADER));
	CHECKGL(glShaderSource(vs, 1, (const GLchar**)&vs_source, NULL));
	CHECKGL(glCompileShader(vs));
	checkGlShader(vs, "(vs) " __FILE__, __LINE__);
	CHECKGL(glAttachShader(prog, vs));

	GLuint fs = CHECKGL_V(glCreateShader(GL_FRAGMENT_SHADER));
	CHECKGL(glShaderSource(fs, 1, (const GLchar**)&fs_source, NULL));
	CHECKGL(glCompileShader(fs));
	checkGlShader(fs, "(fs) " __FILE__, __LINE__);
	CHECKGL(glAttachShader(prog, fs));

	CHECKGL(glLinkProgram(prog));
	checkGlProgram(prog, __FILE__, __LINE__);

	draw();

	while (1)
	{
		SDL_Event event;

		if (!SDL_PollEvent(&event)) {
			draw();
			CHECK(SDL_WaitEvent(&event), != 0);
		}

		if (event.type == SDL_QUIT)
			break;

		if (event.type == SDL_MOUSEMOTION && (event.motion.state & SDL_BUTTON(1)) != 0) {
			theta += event.motion.yrel / 100.0;
			phi += event.motion.xrel / 100.0;
		}
	}

	return 0;
}

