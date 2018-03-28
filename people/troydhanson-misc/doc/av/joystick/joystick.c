#include "SDL/SDL.h"
#define JOYAXIS_MAX 32768

int main(int argc, char *argv[]) {
  int i, n, a,b,l, w;
  SDL_Joystick *j;
  SDL_Event e;

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr,"SDL init failed: %s\n", SDL_GetError());
    return -1;
  }

  n = SDL_NumJoysticks();
  if (n==0) {fprintf(stderr, "No joystick\n"); return 0;}
  for(i=0; i<n; i++) {
    fprintf(stderr,"Joystick %d: %s\n", i+1, SDL_JoystickName(i));
  }

  j = SDL_JoystickOpen(0); // open the first one 
  if (!j) {fprintf(stderr,"can't open joystick: %s\n", SDL_GetError()); return -1;}
  a = SDL_JoystickNumAxes(j);
  b = SDL_JoystickNumButtons(j);
  l = SDL_JoystickNumBalls(j);
  fprintf(stderr, "axes: %d, buttons: %d, balls: %d\n", a, b, l);

  fprintf(stderr,"detecting motion. press joystick button to exit\n");
  while ( (w=SDL_WaitEvent(&e)) != 0) {

    switch (e.type) {
    case SDL_JOYAXISMOTION: 
      if ((e.jaxis.value < -3200) || (e.jaxis.value > 3200)) // reduce tweakiness
        fprintf(stderr,"axis %d (%s) motion: %d%%\n", e.jaxis.axis, 
          (e.jaxis.axis==0?"left-right":
          ((e.jaxis.axis==1?"up-down":"other"))),
          (int)(e.jaxis.value*100.0/JOYAXIS_MAX));
      break;

    case SDL_JOYBUTTONDOWN: 
      fprintf(stderr,"button %d pressed\n", e.jbutton.button);
      goto done;
      break;

    case SDL_JOYHATMOTION: 
      fprintf(stderr,"hat motion\n");
      break;

    case SDL_QUIT:
      goto done;
      break;
    }
  }

 done:
  SDL_JoystickClose(j);
}
