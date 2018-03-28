#!/bin/bash

help() {
	echo "Usage: $0 { dvdplayer | cmx_mp3_1k } <input-file> <output-file>" 2>&1
	echo "       $0 {mode}_dir <input-directory> <output-directory>" 2>&1
	exit 1
}

if [ $# != 3 ]; then
	help
fi

if [ -f "$3" ]; then
	echo 'Error: The target file does exist already!' 2>&1
	exit 1
fi

case "$1" in
	dvdplayer)
		# first try - usually this works quite fine
		exec mencoder -o "$3" -vf scale=576:304 -ovc lavc -oac lavc -lavcopts vcodec=msmpeg4:acodec=mp3 "$2" ;;
	cmx_mp3_1k)
		# for a "CMX MP3 1000" (mencoder cmdline traced from CMX "AVIConvert" windows tool)
		exec mencoder -noodml "$2" -of avi -o "$3" -ofps 20 -vf-add scale=320:240 \
				-vf-add expand=320:240:-1:-1:1 -srate 44100 -ovc xvid \
				-xvidencopts bitrate=600:max_bframes=0:quant_type=h263:me_quality=6 \
				-oac lavc -lavcopts acodec=mp2:abitrate=128 ;;
	*_dir)
		mode="$( echo "$1" | sed -r 's,_dir$,,')"
		while read ifn; do
			[ -f "$2/$ifn" ] || continue
			ofn="$( echo "$ifn" | sed -r 's,\.[^.]*$,,;')".avi
			echo "=== $2/$ifn -> $3/$ofn ==="
			bash "$0" $mode "$2/$ifn" "$3/$ofn"
		done < <( ls "$2" ) ;;
	*)
		help
esac

