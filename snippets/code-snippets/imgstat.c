/**
 * Prints minimal statistics on all the images given to it on the 
 * command line.
 *
 * Written by Tyler Montbriand.  Freeware.
 */
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

int main(int argc, char *argv[])
{
  int failed=0;
  int n;
  if(argc==1)
  {
    fprintf(stderr,"syntax:  imgstat img1 [img2 ...]\n");
    return(1);
  }

  for(n=1; n<argc; n++)
  {
    SDL_Surface *s=IMG_Load(argv[n]);
    if(s==NULL)
    {
      failed++;
      fprintf(stderr,"Couldn't open %s: %s\n", argv[n], SDL_GetError());
      continue;
    }

    printf("%s\t%d\t%d\t%d\n",
      argv[n],
      s->w,
      s->h,
      s->format->BitsPerPixel);

    SDL_FreeSurface(s);
  }

  return(0);
}
