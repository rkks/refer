CFLAGS=-Wall -g
GST_CFLAGS := $(shell pkg-config --cflags gstreamer-1.0)
GST_LIBS := $(shell pkg-config --libs gstreamer-1.0)

play-ogg: play-ogg.c
	$(CC) $(CFLAGS) $(GST_CFLAGS) -o $@ $< $(GST_LIBS)

.PHONY: clean

clean:
	rm -f play-ogg *.o
