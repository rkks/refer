CFLAGS=`sdl-config --cflags` -I./include
LDFLAGS=`sdl-config --libs` -lSDL_net
OBJS=SDL_rwfd.o SDL_rwsock.o sdlsock.o
all:sdlsock sdlsock-static
sdlsock:$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

sdlsock-static:SDL_rwfd.o SDL_rwsock.o sdlsock.o libSDL.a libSDL_net.a
	$(CC) $(CFLAGS) -c -mcpu=i386 SDL_rwfd.c SDL_rwsock.c sdlsock.c
	$(CC) -mcpu=i386 -lpthread -L./ $(OBJS) libSDL.a libSDL_net.a -o $@
	strip $@
clean:
	rm -Rf $(OBJS) sdlsock-static sdlsock

