#!/bin/sh

if [ ! -s youscope-wave.flac ]; then
	wget -O youscope-wave.flac "http://koti.kapsi.fi/jpa/stuff/other/youscope-wave.flac"
fi

if [ ! -s youscope-wave.raw ]; then
	flac -f -d --force-raw-format --sign=signed --endian=little -o youscope-wave.raw youscope-wave.flac
fi

./oszi -i -d binary < youscope-wave.raw

