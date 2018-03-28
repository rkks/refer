/**
 * I coded this when I knew little about OpenGL.
 * It was supposed to display a rotating torus.
 * IT DOES NOT.  But the result is amusing.
 */
  
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MINOR_SEGMENTS 20
#define MAJOR_SEGMENTS 10

#define MINOR_RAD       0.5f
#define MAJOR_RAD       2.0f

#define POINT_X         0
#define POINT_Y         1
#define POINT_Z         2

#define RADIAN_MAX      (2.0f*3.14159)

#define RADIAN_SCALE(val,max) ((val*RADIAN_MAX)/max)
#define DEGREE_SCALE(val,max) ((val*360.0f)/max)

/**
 *  Macro that prints your error message to stderr, along with
 *  SDL's last error, before exiting.
 *
 *  Like, SDL_Die("Couldn't init video");
 *  might print "Couldn't init video: No video driver available"
 */
#define SDL_Die(message) { fprintf(stderr,"%s: %s\n",message,SDL_GetError());  exit(1); }

int setup_opengl(int x, int y, int width, int height );

/**
 *  Enters an event loop, processing SDL_Events until
 *  it recieves an SDL_QUIT event, or is told to stop.
 *
 *  It takes two optional function pointers as arguments.
 *  If you do not want to use these functions, give it
 *  NULL instead.
 *
 *  handler is called, if available, to process SDL_Events
 *  it gets other than SDL_QUIT.  Returning a value less
 *  than zero from handler will break the loop.
 *
 *  idle is called, if available, whenever the event loop
 *  is not busy processing events.  Returning a value less
 *  than zero from idle will break the loop.
 */
int eventloop(int (*handler)(SDL_Event *), int(*idle)());

/* draws a rotating cube! */
int draw();

/* will draw rotating donut */
int drawtorus();

int main(int argc, char *argv[])
{
  SDL_Surface *screen=NULL; /* Our main video surface */
  
  if(SDL_Init(SDL_INIT_VIDEO)<0)
    SDL_Die("Couldn't initialize SDL");

  atexit(SDL_Quit); /* Have SDL_Quit called automatically at program exit */

  screen=SDL_SetVideoMode(320,240,24,SDL_ANYFORMAT|SDL_OPENGL);
  if(screen==NULL)
    SDL_Die("Couldn't set video mode");
    
  if(setup_opengl(0,0,0,0)<0) /* Use entire screen for OpenGL viewport */
    SDL_Die("Couldn't set up viewport");

  if(eventloop(NULL,drawtorus)<0)
    SDL_Die("Event loop failed somehow");
    
  return EXIT_SUCCESS;
}

int drawtorus()
{
  int n;
  static GLfloat minor_points[MINOR_SEGMENTS][3];
  static GLubyte red[]    = { 255,   0,   0, 255 };
  static GLubyte blue[]   = {   0,   0, 255, 255 };
  static GLubyte green[]  = {   0, 255,   0, 255 };
  
  
  static int init=0;
  static Sint32 tick;
  static double aoff=0.0f;

  if(!init)
  {
    for(n=0; n<MINOR_SEGMENTS; n++)
    {
      double a=RADIAN_SCALE(n,MINOR_SEGMENTS);

      minor_points[n][POINT_X]=MAJOR_RAD+(MINOR_RAD*cos(a));
      minor_points[n][POINT_Y]=MINOR_RAD*sin(a);
      minor_points[n][POINT_Z]=0.0f;
    }
    tick=SDL_GetTicks();
    init=1;
  }

  aoff+=(SDL_GetTicks()-tick)/10.0f;
  while(aoff>360.0f) aoff-=360.0f;
  
  tick=SDL_GetTicks();
  
  /* Clear the color and depth buffers. */
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* We don't want to modify the projection matrix. */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );

  /* Move down the z-axis. */
  glTranslatef( 0.0, 0.0, -5.0 );   
  
  for(n=0; n<MAJOR_SEGMENTS; n++)
  {
    int m;
    double a2=DEGREE_SCALE(n,MAJOR_SEGMENTS)+aoff;
    while(a2>360.0f) a2-=360.0f;

    glRotatef(90.0f,1.0f,0.0f,0.0f);
    glRotatef(a2,0.0f,1.0f,0.0f);

    glBegin(GL_LINES);

    glColor4ubv( green );
    glVertex3f(0.0f,0.0f,0.0f);
    glVertex3f(MAJOR_RAD,0.0f,0.0f);
    
    for(m=0; m<MINOR_SEGMENTS; m++)
    {      
      glColor4ubv( red );
      glVertex3f( minor_points[m][POINT_X],minor_points[m][POINT_Y],minor_points[m][POINT_Z]);
    }

    glEnd();    
  }

  SDL_GL_SwapBuffers( );

  return(0);
}

int draw()
{
    /**
     * our angle of rotation.  static means it's global.. value will be preserved
     * across function calls.
     */
    static float angle = 0.0f;

    /*
     * EXERCISE:
     * Replace this awful mess with vertex
     * arrays and a call to glDrawElements.
     *
     * EXERCISE:
     * After completing the above, change
     * it to use compiled vertex arrays.
     *
     * EXERCISE:
     * Verify my windings are correct here ;).
     */
    static GLfloat v0[] = { -1.0f, -1.0f,  1.0f };
    static GLfloat v1[] = {  1.0f, -1.0f,  1.0f };
    static GLfloat v2[] = {  1.0f,  1.0f,  1.0f };
    static GLfloat v3[] = { -1.0f,  1.0f,  1.0f };
    static GLfloat v4[] = { -1.0f, -1.0f, -1.0f };
    static GLfloat v5[] = {  1.0f, -1.0f, -1.0f };
    static GLfloat v6[] = {  1.0f,  1.0f, -1.0f };
    static GLfloat v7[] = { -1.0f,  1.0f, -1.0f };    
        
    static GLubyte red[]    = { 255,   0,   0, 255 };
    static GLubyte green[]  = {   0, 255,   0, 255 };
    static GLubyte blue[]   = {   0,   0, 255, 255 };
    static GLubyte white[]  = { 255, 255, 255, 255 };
    static GLubyte yellow[] = {   0, 255, 255, 255 };
    static GLubyte black[]  = {   0,   0,   0, 255 };
    static GLubyte orange[] = { 255, 255,   0, 255 };
    static GLubyte purple[] = { 255,   0, 255,   0 };

    /* Clear the color and depth buffers. */
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    /* We don't want to modify the projection matrix. */
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    /* Move down the z-axis. */
    glTranslatef( 0.0, 0.0, -5.0 );    
    
    /* Rotate. */
    glRotatef( angle, 0.0, 1.0, 0.0 );

    if( ++angle > 360.0f )
      angle-=360.0f;
  
    /* Send our triangle data to the pipeline. */
    glBegin( GL_TRIANGLES );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( blue );
    glVertex3fv( v2 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( black );
    glVertex3fv( v5 );
    glColor4ubv( purple );
    glVertex3fv( v7 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( white );
    glVertex3fv( v3 );

    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( blue );
    glVertex3fv( v2 );
    glColor4ubv( orange );
    glVertex3fv( v6 );

    glColor4ubv( white );
    glVertex3fv( v3 );
    glColor4ubv( orange );
    glVertex3fv( v6 );
    glColor4ubv( purple );
    glVertex3fv( v7 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( red );
    glVertex3fv( v0 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );

    glColor4ubv( green );
    glVertex3fv( v1 );
    glColor4ubv( yellow );
    glVertex3fv( v4 );
    glColor4ubv( black );
    glVertex3fv( v5 );

    glEnd( );

    /*
     * EXERCISE:
     * Draw text telling the user that 'Spc'
     * pauses the rotation and 'Esc' quits.
     * Do it using vetors and textured quads.
     */

    /*
     * Swap the buffers. This this tells the driver to
     * render the next frame from the contents of the
     * back-buffer, and to set all rendering operations
     * to occur on what was the front-buffer.
     *
     * Double buffering prevents nasty visual tearing
     * from the application drawing on areas of the
     * screen that are being updated at the same time.
     */
    SDL_GL_SwapBuffers( );

    return(0);
}

/**
 * Taken from the SDL website, and modified by ty to assume
 * some default values and do some sanity checking
 */
int setup_opengl(int x, int y, int width, int height )
{
    float ratio=0.0f; /* aspect ratio, width/height */
    
    SDL_Surface *screen=SDL_GetVideoSurface(); /* video surface */
    
    if(screen==NULL)                      return(-1); /* No video             */    
    else if(!(screen->flags&SDL_OPENGL))  return(-1); /* OpenGL not turned on */
    
    /* less-than or equal to zero values mean use entire window/screen */
    if((width<=0)||(height<=0))
    {
      x=0;
      y=0;      
      width=screen->w;      
      height=screen->h;
    }

    ratio = (float) width / (float) height;

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( x, y, width, height );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     *
     * (note from ty) Why?  Because gluPerspective can be a real bitch
     * to try and use because silly installations might not have it.
     */
    gluPerspective( 60.0, ratio, 1.0, 8.0 );

    return(0);
}

int eventloop(int (*handler)(SDL_Event *), int(*idle)())
{
  int running=1;
  SDL_Event event;

  while(running) /* Main event loop, keep running until false */
  {
    while(SDL_PollEvent(&event)) /* Retrieve events, looping until que is empty */
      switch(event.type)
      {
      case SDL_QUIT: /* SDL generates this when the close X is clicked */
        running=0;       /* break the loop */
        break;

      default:       /* Every other event */
        if(handler!=NULL)       /* Do we have an event handler? */
          if(handler(&event)<0)   /* Did it return error?       */
            running=0;              /* If so, break the loop    */
        break;
      }

    SDL_Delay(10); /* Brief pause to not hog 100% CPU power */

    if(idle!=NULL) /* Do we have an idle function? */
      if(idle()<0)   /* Did it return error?       */
        running=0;     /* If so break loop         */
  }

  return(0);
}
