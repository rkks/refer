#!/bin/bash
#
# Build GCC and Binutils cross-tools for ARM embedded development
#
# Copyright (C) 2010  Clifford Wolf <clifford@clifford.at>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

set -xe

basedir="$PWD"
prefix="$basedir/arm-tools"
target="arm-unknown-elf"

binutilsver=2.20.1
gccver=4.5.0

if [ ! -d binutils-$binutilsver ]; then
	tar xvjf binutils-$binutilsver.tar.bz2
fi

if [ ! -d gcc-$gccver ]; then
	tar xvjf gcc-$gccver.tar.bz2
fi

mkdir -p "$prefix"/src/{binutils,gcc}-build

cd "$prefix"/src/binutils-build/
"$basedir"/binutils-$binutilsver/configure --prefix="$prefix" --target="$target"
make; make install

cd "$prefix"/src/gcc-build/
"$basedir"/gcc-$gccver/configure --prefix="$prefix" --target="$target" --enable-languages=c --disable-libssp
make; make install

