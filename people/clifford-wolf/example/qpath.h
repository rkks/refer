/*
 *  Lookup QObjects in Qt programs using their path in the object tree
 *
 *  Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
 *  USA
 *
 *
 *  The Q_PATH() macro defined in this file can be used like the Q_CHILD()
 *  macro from Qt, but with the addition that it can be used to lookup
 *  entire paths in the object tree.
 *
 *  The elements in this paths are delimitted using a slashes. So the slash
 *  is forbidden in QObject names when using this macro. A double slash makes
 *  the macro do a recursive lookup and the special names '.' and '..' may be
 *  used to refer to an object itself or its parent.
 *
 *  Example:
 *    QString searchString = Q_PATH(searchwin, QLineEdit, "//search_term")->text();
 *
 */

#ifndef QPATH_H
#define QPATH_H 1

#include <qobject.h>

#undef QT_LOOKUP_PATH_DEBUG

static QObject *qt_lookup_path_element(QObject * super, const char *name, int namelen, bool recursive)
{
	QObject *next;

	if (namelen < 0)
		for (namelen = 0; name[namelen]; namelen++)
			/* nothing */ ;

	if (namelen == 1 && name[0] == '.') {
#ifdef QT_LOOKUP_PATH_DEBUG
		printf("path element: self\n");
#endif
		next = super;
		goto out;
	}

	if (namelen == 2 && name[0] == '.' && name[1] == '.') {
#ifdef QT_LOOKUP_PATH_DEBUG
		printf("path element: self\n");
#endif
		next = super->parent();
		goto out;
	}

	char namebuf[namelen + 1];
	for (int i = 0; i < namelen; i++)
		namebuf[i] = name[i];
	namebuf[namelen] = 0;

#ifdef QT_LOOKUP_PATH_DEBUG
	printf("path element: %srecursive search for '%s'\n", recursive ? "" : "non-", namebuf);
#endif
	next = super->child(namebuf, NULL, recursive);

      out:
#ifdef QT_LOOKUP_PATH_DEBUG
	if (next)
		printf("  `- %s (%s)\n", next->name("*unnamed*"), next->className());
	else
		printf("  `- (NULL)\n");
#endif

	return next;
}

static QObject *qt_lookup_path(QObject * super, const char *path)
{
	bool recursive = false;
	while (*path == '/') {
		recursive = true;
		path++;
	}
	for (const char *slash = path; *slash; slash++) {
		if (*slash == '/')
			return qt_lookup_path(qt_lookup_path_element(super, path, slash - path, recursive), slash + 1);
	}
	return qt_lookup_path_element(super, path, -1, recursive);
}

#define Q_PATH(super,type,path) \
	((type*)qt_lookup_path(super, path)->qt_cast(#type))

#endif

