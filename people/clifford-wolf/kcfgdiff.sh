#!/bin/bash
#
#  kcfgdiff.sh - nice diffs of linux kernel config files
#  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or

diffopts=()

while [ "$#" != "2" ]; do
        if [ ".$1" = ".-u" -o ".$1" = ".-c" -o ".$1" = "-b" -o ".$1" = ".-w" ]; then
                # echo "Ignoring known diff option: $1" >&2
                shift; continue
        fi
        if [ ".$1" = ".-L" ]; then
                diffopts=("${diffopts[@]}" "$1" "$2")
                # echo "Passing -L diff option: '$1' '$2'" >&2
                shift; shift; continue
        fi
        # echo "Ignoring unknown diff argument: $1" >&2
        shift
done

preformat_config() {
        grep -E '^[^#]|^#.* is not set$' "$1" | \
                sed 's,^[^#],: &,' | sort | sed 's,: ,,'
}

diff -U 0 "${diffopts[@]}" <( preformat_config "$1" ) <( preformat_config "$2" ) | sed 's,^@.*,,'

