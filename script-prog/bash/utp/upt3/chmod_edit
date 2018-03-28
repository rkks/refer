#!/bin/sh
# chmod_edit - Make a file writable, edit it, and remove write
#              permission when finished.

# add write permission to the files you specify
chmod u+w "$@"

# edit the files; use vi if the environment variable VISUAL is not defined
${VISUAL=vi} "$@"

# remove write permission
chmod -w "$@"
