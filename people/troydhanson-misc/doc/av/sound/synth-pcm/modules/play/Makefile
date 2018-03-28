OBJS=play 
BASE=../base
all: $(OBJS)

CFLAGS=-Wall -I$(BASE)/include
CFLAGS+=-g 

GST_CFLAGS := $(shell pkg-config --cflags gstreamer-1.0 glib-2.0 gio-2.0)
GST_LIBS := $(shell pkg-config --libs gstreamer-1.0 glib-2.0 gio-2.0)

gst.o: gst.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) -c $< 

play.o: play.c
	$(CC) $(CFLAGS) -pthread -c $< 

play: play.o gst.o $(BASE)/tpl.o
	$(CC) $(CFLAGS) $(GST_CFLAGS) -o $@ $^ $(GST_LIBS) 

.PHONY: clean

clean:
	rm -f $(OBJS) *.o
