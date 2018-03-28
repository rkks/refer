#!/bin/bash
#
#  Copyright (C) 2009  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#
#  .. This is a collection of packages I like to install on my kubuntu
#  workstations right after installing the base system.
#

# Extra packages to be installed from source:
#  http://github.com/jeremysanders/okularplugin
#  http://asco.sourceforge.net/
#  http://www.opencsg.org/

# Also install kernel api man pages:
#  in kernel source> make mandocs && sudo make installmandocs

package_list=""
add() { package_list="$package_list $*"; }

# GIT
add git-core
add git-svn
add git-doc

# Subversion
add subversion
add subversion-tools

# QEMU
add qemu-kvm
add qemu-kvm-extras

# The TeX/LaTeX stuff
add texlive
add texlive-latex-extra
add texlive-generic-extra
add texlive-lang-german
add psutils a2ps
add poppler-utils pdfjam

# Math Stuff
add gnuplot
add kmplot
add maxima xmaxima wxmaxima
add octave3.2 octave3.2-info qtoctave
add python-numpy python-scipy reinteract

# Multimedia
add vlc
add mplayer
add mencoder
add ffmpeg
add mesa-utils
add glew-utils
add gimp
add inkscape
add dia

# Audio/Synth
add ams
add bristol

# Driver for NVidia card (enable if there is one)
add nvidia-current
add nvidia-current-dev

# Happy on the command line
add vim
add mmv
add tree
add rlwrap
add bash-completion
add pv

# AVR development
add gcc-avr
add binutils-avr
add avr-libc 
add arduino
add avrdude

# Other uC development
add sdcc
add sdcc-ucsim

# C/C++ Development
add gcc
add gcc-doc
add cpp-doc
add gdb
add gdb-doc
add valgrind
add binutils
add binutils-doc
add glibc-doc
add make
add make-doc
add cmake
add automake
add autoconf
add bison
add flex
add swig
add indent
add qt4-doc
add qt4-doc-html
add qt4-dev-tools
add libqt4-dev
add kdelibs5-dev
add libcgal-dev
add libreadline6-dev
add libglut3-dev
add libeigen2-dev
add libpcre3-dev
add libpq-dev
add libsqlite3-dev
add libxml2-dev
add libxslt-dev
add libsdl1.2-dev
add libglew1.5-dev
add libcurl4-openssl-dev
add uuid-dev
add libfann1-dev
add libsdl-image1.2-dev
add libsdl-ttf2.0-dev
add libfftw3-dev
add portaudio19-dev
add libpth-dev
add libgsl0-dev
add gsl-doc-info
add libwxbase2.8-dev
add libwxgtk2.8-dev
add libcairo2-dev
add libpango1.0-dev
add libmpc-dev
add libdb4.8-dev
add libpgapack-serial1
add libusb-dev

# Python development
add ipython
add python-dev
add python2.6-dev
add python-sip-dev
add python-sip4-dev
add python-qt4-dev
add python-qt4-doc
add python-qt4-gl
add pyqt4-dev-tools

# QCAD
add qcad
add qcad-data
add partlibrary

# Electronics Development
add gtkwave
add iverilog
add gplcver
add qucs
add gnucap
add eagle
add gerbv

# Various Desktop stuff
add kubuntu-firefox-installer
add plasma-widget-cpuload
add openjdk-6-jdk openjdk-6-doc
add icedtea6-plugin
add kruler

# Win32 stuff
add wine
add gcc-mingw32
add unrar
add fatsort

# Games
add joystick
add kshisen
add bzflag
add overkill
add frozen-bubble
add flightgear
add supertux
add pingus

# System/Network
add iotop
add wireshark
add genromfs
add ethtool
add numactl
add smartmontools

# Debian tools
add apt-file
add checkinstall
add equivs

# Other tools
add xmlto

# Install all this stuff
apt-get install $package_list

