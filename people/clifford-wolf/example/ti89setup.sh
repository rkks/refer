#!/bin/bash
#
# Little script for installing usefull tools on a TI89 calculator.
#
# Tip: A TI89 can be reset by taking out a battery and holding
# the keys (-) and ) down while putting the battery back in.

mkdir -p ti89apps; cd ti89apps
for url in \
	http://education.ti.com/downloads/files/89/dvmepro.89k \
	http://education.ti.com/downloads/files/89/tipolyrt.89k \
	http://education.ti.com/downloads/files/89/sketchpad.89k \
	http://education.ti.com/downloads/files/89/ticsheet.89k \
	http://education.ti.com/downloads/files/89/dveepro.89k \
	http://education.ti.com/downloads/files/89/tismlteq.89k \
	http://education.ti.com/downloads/files/89/smg200.89k \
	http://education.ti.com/downloads/files/89/ticabri.89k \
	http://education.ti.com/downloads/files/89/calctool.89k
do
	file="${url##*/}"
	read -p "Do you want to install $file now? " in
	[[ "$in" == y* ]] || [[ "$in" == Y* ]] || \
	[[ "$in" == j* ]] || [[ "$in" == J* ]] || continue
	[ -f $file ] || wget $url
	while true; do
		sudo tilp $file
		read -p "Was the upload of $file to the TI89 successful? " in
		[[ "$in" == y* ]] && break
		[[ "$in" == Y* ]] && break
		[[ "$in" == j* ]] && break
		[[ "$in" == J* ]] && break
	done
done

