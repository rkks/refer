/*
 *  Tiny editor for C code with some special features
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef CEDIT_H
#define CEDIT_H

#include <qwidget.h>
#include <q3mainwindow.h>
#include <qmenubar.h>
#include <q3textedit.h>
#include <qprinter.h>
#include <q3syntaxhighlighter.h>
#include <qregexp.h>
#include <qtimer.h>
#include <q3listview.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QCloseEvent>

/* *INDENT-OFF* */

class Editor
:public Q3MainWindow
{
	Q_OBJECT
	
      public:
	Editor(QWidget * parent = 0, const char *name = "cedit");
	~Editor();
	
	bool loadFile(const QString & fileName);
	bool saveFile(const QString & fileName);
	void updateTemplate(QString &c, QString &d);
	
      public slots:
	void newDoc();
	bool load();
	bool save();
	bool saveAs();
	void print();
	void font();
	
      protected:
	void closeEvent(QCloseEvent *);
	
      private slots:
	void textChanged();
	void returnPressed();
	void updateStatusBar();
	void insert_template();
	void indent_apply();
	void indent_check();
	void indent_tick();
	void compile_prog();
	void run_prog();
	void handbook();
	void about();
	void find();
	void find_next();
	void find_next_autoclose();
	void find_prev();
	void find_replace_this();
	void find_replace_all();
	void find_dock(bool on);
	void goto_line();
	
      private:
	QString get_replace_string();
	bool find_run(bool reverse, bool autoclose, bool wrapover_ok);
	void indent_backend(int apply_mode);
	void load_templates();
	
	Q3TextEdit *e, *c, *ie;
	Q3ListView *tpl_list;
	Q3TextEdit *tpl_comment;
	Q3TextEdit *tpl_preview;
	Q3DockWindow *searchwin;
	QRegExp rx_search;
#ifndef QT_NO_PRINTER
	QPrinter printer;
#endif
	QString filename;
	bool changed;
	
	bool indent_timer_enable;
	QTimer *indent_timer;
	int indent_countdown;
	QString indent_error;
};

class C_Highlighter
:public Q3SyntaxHighlighter
{
      public:
	C_Highlighter(Q3TextEdit * textEdit);
	int highlightParagraph(const QString & text, int endStateOfLastPara);
      private:
	struct HighlightingRule {
		QRegExp pattern;
		QColor color;
		bool has_dollar;
	};
	Q3ValueList < HighlightingRule > highlightingRules;
	QRegExp comment_is_placeholder_pattern;
};

class Comment_Highlighter
:public Q3SyntaxHighlighter
{
      public:
	Comment_Highlighter(Q3TextEdit * textEdit);
	int highlightParagraph(const QString & text, int endStateOfLastPara);
};

class Template_Item
:public Q3ListViewItem
{
      private:
	QString template_comment, template_data;
	Editor *editor;
      public:
	Template_Item(Q3ListView *parent, QString l, Editor *e, QString c, QString d);
	Template_Item(Q3ListViewItem *parent, QString l, Editor *e, QString c, QString d);
	Template_Item(Q3ListView *parent, Q3ListViewItem *after, QString l, Editor *e, QString c, QString d);
	Template_Item(Q3ListViewItem *parent, Q3ListViewItem *after, QString l, Editor *e, QString c, QString d);
      protected:
	virtual void activate();
};

extern "C" int wrindent(const char *input, char **output, char **error);

#define CEDIT_DEFAULT_FONT "Courier [ibm],12,-1,5,75,0,0,0,0,0"
#define CEDIT_INDENT_CHECK_TIMEOUT 5

/* *INDENT-ON* */

#endif
