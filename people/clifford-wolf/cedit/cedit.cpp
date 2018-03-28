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
 *  Features:
 *    - Basic syntax highlighting for C code
 *    - Integration of 'indent' for on-the-fly auto indenting
 *    - Support for program snippets in a template library
 *
 *  Build and install:
 *    $ qmake && make
 *    $ install cedit /usr/local/bin/cedit
 *
 *  Run:
 *    $ cedit example.c
 *
 */

#include "cedit.h"

#include <qapplication.h>
#include <qfile.h>
#include <q3filedialog.h>
#include <q3popupmenu.h>
#include <q3textstream.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <q3paintdevicemetrics.h>
#include <q3ptrlist.h>
#include <qfontdialog.h>
#include <qtextcodec.h>
#include <qlayout.h>
#include <q3dockwindow.h>
#include <q3dockarea.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <q3vgroupbox.h>
#include <qinputdialog.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <Q3ValueList>
#include <Q3GridLayout>
#include <Q3VBoxLayout>
#include <QCloseEvent>

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#  include <wtypes.h>
#  include <shellapi.h>
#else
#  include <signal.h>
#  include <unistd.h>
#endif

void open_url(const char *url)
{
#ifdef WIN32
	int i;
	const char *open_str = "open";
	int url_sz = strlen(url)+1;
	int open_sz = strlen(open_str)+1;
	WCHAR wurl[url_sz];
	WCHAR wopen[open_sz];
	for (i=0; i<url_sz; i++)
		wurl[i] = url[i];
	for (i=0; i<open_sz; i++)
		wopen[i] = open_str[i];
	ShellExecute(GetActiveWindow(), wopen, wurl, NULL, NULL, SW_SHOW);
#else
	sighandler_t sigchld_backup = signal(SIGCHLD, SIG_IGN);
	if (!fork()) {
		execlp("firefox", "firefox", url, NULL);
		_exit(-1);
	}
	signal(SIGCHLD, sigchld_backup);
#endif
}

Editor::Editor(QWidget * parent, const char *name)
:Q3MainWindow(parent, name)
{
	/* Editor Area */
	{
		e = new Q3TextEdit(this, "editor");
		new C_Highlighter(e);
		e->setWordWrap(Q3TextEdit::NoWrap);
		connect(e, SIGNAL(textChanged()), this, SLOT(textChanged()));
		connect(e, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
		connect(e, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateStatusBar()));
		QFont f = QFont(CEDIT_DEFAULT_FONT);
		e->setFont(f);
		setCentralWidget(e);
	}
	
	/* Console Area (as dock window) */
	{
		Q3DockWindow *dw = new Q3DockWindow(this, "dw_console");
		c = new Q3TextEdit(dw, "console");
		c->setReadOnly(true);
		c->setPaper(Qt::darkGray);
		c->append("Welcome! This is the console...\n");
		c->setMinimumWidth(600);
		dw->setWidget(c);
		dw->setResizeEnabled(true);
		dw->setCloseMode(Q3DockWindow::Always);
		dw->setCaption("Console");
		dw->show();
		
		addDockWindow(dw, Qt::DockBottom);
	}
	
	/* Indent Error Status (as dock window) */
	{
		Q3DockWindow *dw = new Q3DockWindow(this, "dw_indent_error");
		ie = new Q3TextEdit(dw, "indent_error");
		ie->setReadOnly(true);
		dw->setWidget(ie);
		dw->setResizeEnabled(true);
		dw->setCloseMode(Q3DockWindow::Always);
		dw->setCaption("Indent Error");
		dw->show();
		
		addDockWindow(dw, Qt::DockBottom);
	}
	
	/* Search & Replace Dialog (as dock window) */
	{
		Q3DockWindow *dw = new Q3DockWindow(this, "dw_search");
		searchwin = dw;
		
		Q3VGroupBox *gb1 = new Q3VGroupBox("Search && Replace Options", dw, "gb1");
		Q3VGroupBox *gb2 = new Q3VGroupBox("Window Options", dw, "gb2");
		
		QLabel *l1 = new QLabel("Search:", dw);
		QLabel *l2 = new QLabel("Replace with:", dw);
		
		QLabel *l3 = new QLabel("", dw, "infoline");
		l3->setAlignment(Qt::AlignCenter);
		l3->setTextFormat(Qt::RichText);
		
		QLineEdit *i1 = new QLineEdit(dw, "search");
		connect(i1, SIGNAL(returnPressed()), this, SLOT(find_next_autoclose()));
		
		QLineEdit *i2 = new QLineEdit(dw, "replace");
		
		QCheckBox *c1 = new QCheckBox("Search using regular expressions", gb1, "regex");
		QCheckBox *c2 = new QCheckBox("Allow regex captures in replace string", gb1, "regex_cap");
		
		c2->setEnabled(false);
		connect(c1, SIGNAL(toggled(bool)), c2, SLOT(setEnabled(bool)));
		
		new QCheckBox("Search case sensitive", gb1, "cs");
		
		QCheckBox *c4 = new QCheckBox("Dock this window", gb2, "dock");
		c4->setChecked(true);
		connect(c4, SIGNAL(toggled(bool)), this, SLOT(find_dock(bool)));
		
		QCheckBox *c5 = new QCheckBox("Close on [ENTER]", gb2, "autoclose");
		c5->setChecked(true);
		
		QPushButton *b1 = new QPushButton("Find Next", dw);
		connect(b1, SIGNAL(clicked()), this, SLOT(find_next()));
		
		QPushButton *b2 = new QPushButton("Find Previous", dw);
		connect(b2, SIGNAL(clicked()), this, SLOT(find_prev()));
		
		QPushButton *b3 = new QPushButton("Replace This", dw);
		connect(b3, SIGNAL(clicked()), this, SLOT(find_replace_this()));
		
		QPushButton *b4 = new QPushButton("Replace All", dw);
		connect(b4, SIGNAL(clicked()), this, SLOT(find_replace_all()));
		
		Q3VBoxLayout *box1 = new Q3VBoxLayout(dw->boxLayout());
		Q3GridLayout *box2 = new Q3GridLayout(box1, 2, 2);
		box2->addWidget(l1, 0, 0);
		box2->addWidget(i1, 0, 1);
		box2->addWidget(l2, 1, 0);
		box2->addWidget(i2, 1, 1);
		Q3HBoxLayout *box3 = new Q3HBoxLayout(box1);
		box3->addWidget(gb1);
		box3->addWidget(gb2);
		box1->addWidget(l3);
		Q3HBoxLayout *box4 = new Q3HBoxLayout(box1);
		box4->addWidget(b1);
		box4->addWidget(b2);
		box4->addWidget(b3);
		box4->addWidget(b4);
		
		dw->setHorizontallyStretchable(true);
		dw->setCloseMode(Q3DockWindow::Always);
		dw->setCaption("Find");
		
		addDockWindow(dw, Qt::DockRight);
		dw->hide();
	}
	
	/* Template Browser (as dock window) */
	{
		Q3DockWindow *dw = new Q3DockWindow(this, "dw_templates");
		Q3VBoxLayout *box1 = new Q3VBoxLayout(dw->boxLayout());
		Q3HBoxLayout *box2 = new Q3HBoxLayout(box1);
		
		tpl_list = new Q3ListView(dw, "template_list");
		tpl_list->addColumn("Template Tree");
		tpl_list->setSorting(-1);
		box2->addWidget(tpl_list);
		
		Q3VBoxLayout *box3 = new Q3VBoxLayout(box2);
		QLabel *l1 = new QLabel(dw);
		l1->setText("Template Description:");
		box3->addWidget(l1);
		
		tpl_comment = new Q3TextEdit(dw, "template_comment");
		tpl_comment->setReadOnly(true);
		new Comment_Highlighter(tpl_comment);
		box3->addWidget(tpl_comment);
		
		QLabel *l2 = new QLabel(dw);
		l2->setText("Template Code:");
		box1->addWidget(l2);
		
		tpl_preview = new Q3TextEdit(dw, "template_preview");
		tpl_preview->setReadOnly(true);
		new C_Highlighter(tpl_preview);
		QFont f = QFont(CEDIT_DEFAULT_FONT);
		tpl_preview->setFont(f);
		box1->addWidget(tpl_preview);
		
		QPushButton *b1 = new QPushButton(dw);
		b1->setText("Insert template at cursor position");
		connect(b1, SIGNAL(clicked()), this, SLOT(insert_template()));
		box1->addWidget(b1);
		
		tpl_list->setRootIsDecorated(true);
		load_templates();
		
		dw->setResizeEnabled(true);
		dw->setCloseMode(Q3DockWindow::Always);
		dw->setCaption("Template Browser");
		dw->show();
		
		addDockWindow(dw, Qt::DockRight);
	}
	
	/* File menu */
	{
		Q3PopupMenu *menu = new Q3PopupMenu();
		Q_CHECK_PTR(menu);
		menuBar()->insertItem("&File", menu);
		
		menu->insertItem("&New", this, SLOT(newDoc()), Qt::CTRL + Qt::Key_N);
		menu->insertItem("&Open...", this, SLOT(load()), Qt::CTRL + Qt::Key_O);
		menu->insertItem("&Save", this, SLOT(save()), Qt::CTRL + Qt::Key_S);
		menu->insertItem("&Save As...", this, SLOT(saveAs()), 0);
#if !defined QT_NO_FONTDIALOG || !defined QT_NO_PRINTER
		menu->insertSeparator();
#  ifndef QT_NO_FONTDIALOG
		menu->insertItem("&Font...", this, SLOT(font()), 0);
#  endif
#  ifndef QT_NO_PRINTER
		menu->insertItem("&Print...", this, SLOT(print()), Qt::CTRL + Qt::Key_P);
#  endif
#endif
		menu->insertSeparator();
		menu->insertItem("&Close", this, SLOT(close()), Qt::CTRL + Qt::Key_W);
		menu->insertItem("&Quit", qApp, SLOT(closeAllWindows()), Qt::CTRL + Qt::Key_Q);
	}
	
	/* Edit menu */
	{
		Q3PopupMenu *menu = new Q3PopupMenu();
		Q_CHECK_PTR(menu);
		menuBar()->insertItem("&Edit", menu);
		
		menu->insertItem("C&ut", e, SLOT(cut()), Qt::CTRL + Qt::Key_X);
		menu->insertItem("&Copy", e, SLOT(copy()), Qt::CTRL + Qt::Key_C);
		menu->insertItem("&Paste", e, SLOT(paste()), Qt::CTRL + Qt::Key_V);
		menu->insertItem("&Undo", e, SLOT(undo()), Qt::CTRL + Qt::Key_Z);
		menu->insertItem("&Redo", e, SLOT(redo()), Qt::CTRL + Qt::Key_Y);
		menu->insertSeparator();
		menu->insertItem("&Find...", this, SLOT(find()), Qt::CTRL + Qt::Key_F);
		menu->insertItem("Find &Next", this, SLOT(find_next()), Qt::Key_F3);
		menu->insertItem("Find &Previous", this, SLOT(find_prev()), Qt::SHIFT + Qt::Key_F3);
		menu->insertItem("&Replace This", this, SLOT(find_replace_this()), Qt::Key_F4);
		menu->insertItem("&Goto Line...", this, SLOT(goto_line()), Qt::CTRL + Qt::Key_G);
		menu->insertSeparator();
		menu->insertItem("Insert &Template", this, SLOT(insert_template()), Qt::CTRL + Qt::Key_T);
		menu->insertItem("&Indent apply", this, SLOT(indent_apply()), Qt::CTRL + Qt::Key_I);
		menu->insertItem("&Indent check", this, SLOT(indent_check()), Qt::CTRL + Qt::SHIFT + Qt::Key_I);
	}
	
	/* Implement menu */
	{
		Q3PopupMenu *menu = new Q3PopupMenu();
		Q_CHECK_PTR(menu);
		menuBar()->insertItem("&Backend", menu);
		
		menu->insertItem("&Compile", this, SLOT(compile_prog()), Qt::Key_F8);
		menu->insertItem("&Run", this, SLOT(run_prog()), Qt::Key_F9);
	}
	
	/* Windows menu (managed by app) */
	{
		Q3PopupMenu *menu = createDockWindowMenu();
		Q_CHECK_PTR(menu);
		menuBar()->insertItem("&Windows", menu);
	}
	
	/* Help menu */
	{
		Q3PopupMenu *menu = new Q3PopupMenu();
		Q_CHECK_PTR(menu);
		menuBar()->insertItem("&Help", menu);
		
		menu->insertItem("&Handbook", this, SLOT(handbook()), 0);
		menu->insertItem("&About", this, SLOT(about()), 0);
	}
	
	changed = false;
	
	indent_timer = new QTimer(this, "indent_timer");
	connect(indent_timer, SIGNAL(timeout()), this, SLOT(indent_tick()));
	indent_timer_enable = false;
	
	updateStatusBar();
	e->setFocus();
}

Editor::~Editor()
{
}

void Editor::font()
{
#ifndef QT_NO_FONTDIALOG
	bool ok;
	QFont f = QFontDialog::getFont(&ok, e->font());
	if (ok) {
		e->setFont(f);
		tpl_preview->setFont(f);
		// printf("Set font to '%s'.\n", f.toString().ascii());
	}
#endif
}

void Editor::newDoc()
{
	Editor *ed = new Editor;
	ed->resize(900, 700);
	ed->show();
}

bool Editor::load()
{
#ifndef QT_NO_FILEDIALOG
	QString fn = Q3FileDialog::getOpenFileName(QString::null, QString::null, this);
	if (!fn.isEmpty())
		return loadFile(fn);
#endif
	return false;
}

bool Editor::save()
{
	if (filename.isNull())
		return saveAs();
	return saveFile(filename);
}

bool Editor::saveAs()
{
#ifndef QT_NO_FILEDIALOG
	//storing filename (proper save) is left as an exercise...
	QString fn = Q3FileDialog::getSaveFileName(QString::null, QString::null, this);
	if (!fn.isEmpty())
		return saveFile(fn);
#endif
	return false;
}

bool Editor::loadFile(const QString & fileName)
{
	indent_timer_enable = false;
	
	QFile f(fileName);
	if (!f.open(QIODevice::ReadOnly))
		return false;
		
	Q3TextStream t(&f);
	e->setText(t.read());
	f.close();
	
	e->repaint();
	setCaption(fileName);
	
	indent_check();
	changed = false;
	filename = fileName;
	
	return true;
}

bool Editor::saveFile(const QString & fileName)
{
	indent_check();
	
	QFile f(fileName);
	if (!f.open(QIODevice::WriteOnly)) {
		QMessageBox::warning(this, "I/O Error", "The file could not be opened.\n" + fileName);
		return false;
	}
	Q3TextStream t(&f);
	t << e->text();
	f.close();
	
	setCaption(fileName);
	changed = false;
	filename = fileName;
	
	return true;
}

void Editor::updateTemplate(QString & c, QString & d)
{
	tpl_comment->setText(c);
	tpl_preview->setText(d);
	e->setFocus();
}

void Editor::print()
{
	// FIXME
	QMessageBox::warning(this, "Unimplemented Feature", "Sorry! This feature is not implemented yet.\n");
	return;
	
#ifndef QT_NO_PRINTER
	// opens printer dialog
	if (printer.setup(this))
	{
		// we'll set our own margins
		printer.setFullPage(true);
		QPainter p;
		// paint on printer
		p.begin(&printer);
		
		// FIXME: Add text to printer output
		
		// send job to printer
		p.end();
	}
#endif
}

void Editor::closeEvent(QCloseEvent * event)
{
	event->accept();
	
	// the text has been changed
	if (changed)
	{
		switch (QMessageBox::warning(this, "cedit", "Save changes to Document?", tr("&Yes"), tr("&No"), tr("Cancel"), 0, 2))
		{
		case 0:
			// yes
			if (save())
				event->accept();
			else
				event->ignore();
			break;
		case 1:
			// no
			event->accept();
			break;
		default:
			// cancel
			event->ignore();
			break;
		}
	}
}

void Editor::textChanged()
{
	changed = true;
	if (indent_timer_enable) {
		indent_countdown = CEDIT_INDENT_CHECK_TIMEOUT + 1;
		indent_tick();
	}
}

static QString get_indent_from_line(QString l)
{
	int i;
	
	// This is a workaround for the QTextEdit Widget (Qt3.3).
	// It adds a blank to each line in the output of the text(N)
	// method that must be removed here..
	if (l.length() > 0 && l[l.length() - 1] == ' ')
		l.truncate(l.length() - 1);
		
	for (i = 0; i < (int) l.length(); i++)
		if (l[i] != ' ' && l[i] != '\t')
			break;
			
	l.truncate(i);
	return l;
}

void Editor::returnPressed()
{
	int para, pos;
	e->getCursorPosition(&para, &pos);
	
	if (para == 0) {
		/* Wtf? */
		return;
	}
	
	QString prev_line = e->text(para - 1);
	
	int p = prev_line.findRev('{');
	int q = p >= 0 ? prev_line.find('}', p) : -1;
	
	if (p > q) {
		e->insert(get_indent_from_line(prev_line) + QString("\t"));
	} else {
		e->insert(get_indent_from_line(prev_line));
	}
}

void Editor::updateStatusBar()
{
	int para, pos;
	e->getCursorPosition(&para, &pos);
	char message[1024];
	if (indent_timer->isActive()) {
		snprintf(message, 1024, "Cursor position: line=%d, column=%d   [Automatic indent check in %d seconds]", para + 1, pos + 1, indent_countdown);
	} else {
		snprintf(message, 1024, "Cursor position: line=%d, column=%d", para + 1, pos + 1);
	}
	statusBar()->message(message);
}

void Editor::insert_template()
{
	int para, pos;
	e->getCursorPosition(&para, &pos);
	
	QString ind_str = get_indent_from_line(e->text(para));
	
	if ((int) ind_str.length() > pos)
		ind_str.truncate(pos);
		
	QString new_str = tpl_preview->text();
	new_str.replace('\n', QString("\n") + ind_str, false);
	
	e->insert(new_str);
	e->setFocus();
}

void Editor::indent_backend(int apply_mode)
{
	indent_timer->stop();
	indent_timer_enable = false;
	
	int cur_para, cur_idx;
	e->getCursorPosition(&cur_para, &cur_idx);
	
	QString indent_input = e->text();
	char *output, *error;
	
	int rc = wrindent(indent_input.ascii(), &output, &error);
	
	QString indent_output(output);
	QString indent_error(error);
	
	free(output);
	free(error);
	
	if (rc != 0) {
		QString l;
		l.sprintf("Indent returned error status %d.\n", rc);
		indent_error += l;
	}
	
	if (indent_error.isEmpty()) {
		if (QString::compare(indent_output, indent_input) != 0) {
			if (apply_mode) {
				e->selectAll();
				e->insert(indent_output);
				e->setCursorPosition(cur_para, cur_idx);
				e->repaint();
				changed = true;
				goto indent_ok_branch;
			} else {
				ie->setText("Indent check: UPDATES.\n\nPress CTRL+I to apply indent updates.");
				ie->setPaper(Qt::yellow);
			}
		} else {
		      indent_ok_branch:
			ie->setText("Indent check: OK.\n");
			ie->setPaper(Qt::green);
		}
	} else {
		ie->setText(QString("Indent check: ERROR.\n") + indent_error);
		ie->setPaper(Qt::red);
	}
	
	indent_timer_enable = true;
}

void Editor::indent_check()
{
	Editor::indent_backend(0);
}

void Editor::indent_apply()
{
	Editor::indent_backend(1);
	e->setFocus();
}

void Editor::indent_tick()
{
	if (indent_countdown > 0) {
		indent_countdown--;
		indent_timer->start(1000, true);
	} else {
		indent_check();
	}
	updateStatusBar();
}

void Editor::compile_prog()
{
	// FIXME
	QMessageBox::warning(this, "Unimplemented Feature", "Sorry! This feature is not implemented yet.\n");
}

void Editor::run_prog()
{
	// FIXME
	QMessageBox::warning(this, "Unimplemented Feature", "Sorry! This feature is not implemented yet.\n");
}

void Editor::handbook()
{
	open_url("http://www.google.com/");
}

void Editor::about()
{
/* *INDENT-OFF* */
QMessageBox::about(this, "cedit",
"Tiny editor for C code running indent automatically in background\n\n"
"Copyright (C) 2008  Clifford Wolf <clifford@clifford.at>\n\n"
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2 of the License, or\n"
"(at your option) any later version.\n");
/* *INDENT-ON* */
}

void Editor::find()
{
	searchwin->show();
	searchwin->findChild < QLineEdit * >("search")->setFocus();
	searchwin->findChild < QLineEdit * >("search")->selectAll();
}

bool Editor::find_run(bool reverse, bool autoclose, bool wrapover_ok)
{
	int start_para, start_index;
	
	if (e->hasSelectedText()) {
		int paraFrom, indexFrom, paraTo, indexTo;
		e->getSelection(&paraFrom, &indexFrom, &paraTo, &indexTo);
		start_para = reverse ? paraFrom : paraTo;
		start_index = reverse ? indexFrom : indexTo;
	} else
		e->getCursorPosition(&start_para, &start_index);
		
	int current_para = start_para;
	int current_index = start_index;
	
	QString search = searchwin->findChild < QLineEdit * >("search")->text();
	bool regex = searchwin->findChild < QCheckBox * >("regex")->isChecked();
	bool cs = searchwin->findChild < QCheckBox * >("cs")->isChecked();
	
	if (regex)
		rx_search = QRegExp(search, cs);
		
	bool last_line = false;
	bool search_failed = false;
	bool wrapover = false;
	bool found = false;
	
	if (reverse) {
		current_index--;
		if (current_index < 0)
			current_para--;
		if (current_para < 0) {
			wrapover = true;
			current_para = e->paragraphs() - 1;
		}
	}
	
	while (wrapover_ok || !wrapover)
	{
		int start;
		int len;
		
		if (regex) {
			if (!reverse)
				start = rx_search.search(e->text(current_para), current_index);
			else
				start = rx_search.searchRev(e->text(current_para), current_index);
			len = rx_search.matchedLength();
		} else {
			if (!reverse)
				start = e->text(current_para).find(search, current_index, cs);
			else
				start = e->text(current_para).findRev(search, current_index, cs);
			len = search.length();
		}
		
		if (start >= 0) {
			e->setCursorPosition(current_para, start);
			e->ensureCursorVisible();
			if (len > 0)
				e->setSelection(current_para, start, current_para, start + len);
			else
				e->removeSelection();
			found = true;
			break;
		}
		
		if (last_line) {
			search_failed = true;
			break;
		}
		
		if (!reverse) {
			current_index = 0;
			current_para++;
			if (current_para >= e->paragraphs()) {
				current_para = 0;
				wrapover = true;
			}
		} else {
			current_index = -1;
			current_para--;
			if (current_para < 0) {
				current_para = e->paragraphs() - 1;
				wrapover = true;
			}
		}
		
		if (current_para == start_para)
			last_line = true;
	}
	
	if (search_failed) {
		searchwin->findChild < QLabel * >("infoline")->setText("<font color=\"red\">Phrase not found.</font>");
	} else if (wrapover) {
		if (!reverse)
			searchwin->findChild < QLabel * >("infoline")->setText("Reached end of page, continued from top.");
		else
			searchwin->findChild < QLabel * >("infoline")->setText("Reached end of page, continued from bottom.");
	} else
		searchwin->findChild < QLabel * >("infoline")->setText("");
		
	if (autoclose) {
		if (searchwin->findChild < QCheckBox * >("autoclose")->isChecked()) {
			searchwin->hide();
			if (search_failed)
				QMessageBox::warning(this, "Search failed", "Phrase not found.");
			e->setFocus();
		}
	}
	
	return found;
}

void Editor::find_next()
{
	find_run(false, false, true);
}

void Editor::find_next_autoclose()
{
	find_run(false, true, true);
}

void Editor::find_prev()
{
	find_run(true, false, true);
}

QString Editor::get_replace_string()
{
	bool regex = searchwin->findChild < QCheckBox * >("regex")->isChecked();
	bool regex_cap = searchwin->findChild < QCheckBox * >("regex_cap")->isChecked();
	QString replace = searchwin->findChild < QLineEdit * >("replace")->text();
	
	if (!regex || !regex_cap)
		return replace;
		
	QString result("");
	QStringList caplist = rx_search.capturedTexts();
	
	while (1)
	{
		int dpos = replace.find('$');
		if (dpos < 0) {
			result += replace;
			break;
		}
		
		result += replace.left(dpos);
		replace = replace.mid(dpos + 1);
		
		if (replace[0] == '$') {
			result += "$";
			replace = replace.mid(1);
			continue;
		}
		
		if (replace[0] >= '0' && replace[0] <= '9') {
			int len = 1;
			while (replace[len] >= '0' && replace[len] <= '9')
				len++;
			int capnum = replace.left(len).toInt();
			if (capnum < (int) caplist.count())
				result += caplist[capnum];
			replace = replace.mid(len);
			continue;
		}
		
		if (replace[0] == '{' && replace.find('}')) {
			int endpos = replace.find('}');
			if (endpos < 0)
				goto recover;
			bool ok;
			int capnum = replace.mid(1, endpos - 1).toInt(&ok);
			if (!ok)
				goto recover;
			if (capnum < (int) caplist.count())
				result += caplist[capnum];
			replace = replace.mid(endpos + 1);
			continue;
		}
		
	      recover:
		result += "$";
	}
	
	return result;
}

void Editor::find_replace_this()
{
	if (e->hasSelectedText()) {
		QString replace = get_replace_string();
		e->insert(replace);
		// find_run(false, false, true);
	}
}

void Editor::find_replace_all()
{
	int count = 0;
	e->setCursorPosition(0, 0);
	while (find_run(false, false, false)) {
		QString replace = get_replace_string();
		if (replace.length() == 0 && !e->hasSelectedText())
			break;
		e->insert(replace);
		count++;
	}
	QString msg;
	msg.sprintf("Replaced %d times.", count);
	QMessageBox::information(this, "Replace All Results", msg);
}

void Editor::find_dock(bool on)
{
	if (!on)
		searchwin->undock();
	leftDock()->setAcceptDockWindow(searchwin, on);
	rightDock()->setAcceptDockWindow(searchwin, on);
	topDock()->setAcceptDockWindow(searchwin, on);
	bottomDock()->setAcceptDockWindow(searchwin, on);
}

void Editor::goto_line()
{
	bool ok;
	int line;
	e->getCursorPosition(&line, NULL);
	line = QInputDialog::getInteger("Goto Line", "Enter line number:", line + 1, 1, e->paragraphs() - 1, 1, &ok);
	if (ok) {
		e->setCursorPosition(line - 1, 0);
		e->ensureCursorVisible();
	}
}

C_Highlighter::C_Highlighter(Q3TextEdit * textEdit)
:  Q3SyntaxHighlighter(textEdit)
{
	QStringList::iterator it;
	QStringList keywordPatterns;
	keywordPatterns << "\\b(char|const|double|enum|int|long|short|signed|struct|union|unsigned|void)\\b";
	keywordPatterns << "\\b(static|volatile|register|inline|typedef|typeof|sizeof|asm|__[a-zA-Z0-9_]+__)\\b";
	keywordPatterns << "\\b(for|while|do|if|else|switch|case|default|return|break|continue)\\b";
	keywordPatterns << "\\b(u?int_(8|16|32))\\b";
	
	for (it = keywordPatterns.begin(); it != keywordPatterns.end(); ++it) {
		HighlightingRule rule;
		rule.pattern = QRegExp(*it);
		rule.color = Qt::darkMagenta;
		rule.has_dollar = false;
		highlightingRules.append(rule);
	}
	
	/* Grouping and delimiitng chars */
	{
		HighlightingRule rule;
		rule.pattern = QRegExp("[(){},;]");
		rule.color = Qt::darkBlue;
		rule.has_dollar = false;
		highlightingRules.append(rule);
	}
	
	/* Pre-Processord directives */
	{
		HighlightingRule rule;
		rule.pattern = QRegExp("^\\s*#.*");
		rule.color = Qt::darkMagenta;
		rule.has_dollar = false;
		highlightingRules.append(rule);
	}
	
	/* Numbers */
	{
		HighlightingRule rule;
		rule.pattern = QRegExp("\\b[0-9][0-9A-Za-z\\.]*");
		rule.color = Qt::darkRed;
		rule.has_dollar = false;
		highlightingRules.append(rule);
	}
	
	/* Placeholders */
	{
		HighlightingRule rule;
		rule.pattern = QRegExp("/\\* *\\$[0-9A-Za-z\\.]* *\\*/");
		comment_is_placeholder_pattern = rule.pattern;
		rule.color = Qt::red;
		rule.has_dollar = true;
		highlightingRules.append(rule);
		rule.pattern = QRegExp("\\$[0-9A-Za-z][0-9A-Za-z]*");
		highlightingRules.append(rule);
	}
}

int C_Highlighter::highlightParagraph(const QString & text, int endStateOfLastPara)
{
	setFormat(0, text.length(), textEdit()->currentFont(), Qt::black);
	
	bool has_dollar = text.find('$') >= 0;
	Q3ValueList < HighlightingRule >::iterator it;
	for (it = highlightingRules.begin(); it != highlightingRules.end(); ++it) {
		// The 'has_dollar' thing is an ugly hack!
		// For some reason Qt3.3 is evaluating the placeholder regexp
		// so slow that this speeds up everything significantly...
		if ((*it).has_dollar && !has_dollar)
			continue;
		QRegExp rx((*it).pattern);
		for (int length, pos = 0; (pos = rx.search(text, pos)) != -1; pos += length) {
			length = rx.matchedLength();
			setFormat(pos, length, textEdit()->currentFont(), (*it).color);
		}
	}
	
	int state = endStateOfLastPara < 0 ? 0 : endStateOfLastPara;
	int sect_start_index = 0;
	int index;
	
#define update_section()                                                                                                   \
	do {                                                                                                               \
		if (state == 1)                                                                                            \
			/* this is a one-line comment section */                                                           \
			setFormat(sect_start_index, index-sect_start_index+1, textEdit()->currentFont(), Qt::darkGray);    \
		if (state == 2)                                                                                            \
			/* this is a multi-line comment section */                                                         \
			setFormat(sect_start_index, index-sect_start_index+1, textEdit()->currentFont(), Qt::darkGray);    \
		if (state == 3)                                                                                            \
			/* this is a string section */                                                                     \
			setFormat(sect_start_index, index-sect_start_index+1, textEdit()->currentFont(), Qt::darkRed);     \
		if (state == 4)                                                                                            \
			/* this is a char section */                                                                       \
			setFormat(sect_start_index, index-sect_start_index+1, textEdit()->currentFont(), Qt::darkRed);     \
		sect_start_index = index;                                                                                  \
	} while (0)
	
	for (index = 0; index < (int) text.length(); index++) {
		if (state == 0 && text[index] == '/' && text[index + 1] == '/') {
			update_section();
			state = 1;
			index++;
			continue;
		}
		if (state == 0 && text[index] == '/' && text[index + 1] == '*') {
			update_section();
			state = 2;
			index++;
			continue;
		}
		if (state == 0 && text[index] == '\"') {
			update_section();
			state = 3;
			continue;
		}
		if (state == 0 && text[index] == '\'') {
			update_section();
			state = 4;
			continue;
		}
		if (state == 1 && text[index] == '\n') {
			update_section();
			state = 0;
			continue;
		}
		if (state == 2 && text[index] == '*' && text[index + 1] == '/') {
			index++;
			if (comment_is_placeholder_pattern.search(text.mid(sect_start_index, index - sect_start_index + 1), 0) == -1 || comment_is_placeholder_pattern.matchedLength() != index - sect_start_index + 1)
				update_section();
			state = 0;
			continue;
		}
		if ((state == 3 || state == 4) && text[index] == '\\') {
			index++;
			continue;
		}
		if (state == 3 && text[index] == '\"') {
			update_section();
			state = 0;
			continue;
		}
		if (state == 4 && text[index] == '\'') {
			update_section();
			state = 0;
			continue;
		}
	}
	
	if (state == 1) {
		update_section();
		state = 0;
	}
	
	update_section();
#undef update_section

	return state;
}

Comment_Highlighter::Comment_Highlighter(Q3TextEdit * textEdit)
:  Q3SyntaxHighlighter(textEdit)
{
}

int Comment_Highlighter::highlightParagraph(const QString & text, int endStateOfLastPara)
{
	setFormat(0, text.length(), textEdit()->currentFont(), Qt::black);
	
	if (endStateOfLastPara) {
		/* avoid waring for unused 'endStateOfLastPara' */
	}
	
	QRegExp rx("\\$[0-9A-Za-z][0-9A-Za-z]*");
	for (int length, pos = 0; (pos = rx.search(text, pos)) != -1; pos += length) {
		length = rx.matchedLength();
		setFormat(pos, length, textEdit()->currentFont(), Qt::red);
	}
	
	rx = QRegExp("\"[0-9A-Za-z][0-9A-Za-z ]*[0-9A-Za-z]\"");
	for (int length, pos = 0; (pos = rx.search(text, pos)) != -1; pos += length) {
		length = rx.matchedLength();
		setFormat(pos + 1, length - 2, textEdit()->currentFont(), Qt::darkMagenta);
	}
	
	return 0;
}

Template_Item::Template_Item(Q3ListView * parent, QString l, Editor * e, QString c, QString d)
:  Q3ListViewItem(parent, l)
{
	editor = e;
	template_comment = c;
	template_data = d;
}

Template_Item::Template_Item(Q3ListViewItem * parent, QString l, Editor * e, QString c, QString d)
:  Q3ListViewItem(parent, l)
{
	editor = e;
	template_comment = c;
	template_data = d;
}

Template_Item::Template_Item(Q3ListView * parent, Q3ListViewItem * after, QString l, Editor * e, QString c, QString d)
:  Q3ListViewItem(parent, after, l)
{
	editor = e;
	template_comment = c;
	template_data = d;
}

Template_Item::Template_Item(Q3ListViewItem * parent, Q3ListViewItem * after, QString l, Editor * e, QString c, QString d)
:  Q3ListViewItem(parent, after, l)
{
	editor = e;
	template_comment = c;
	template_data = d;
}

void Template_Item::activate()
{
	editor->updateTemplate(template_comment, template_data);
}

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	for (int i = argc <= 1 ? 0 : 1; i < argc; i++) {
		Editor *e = new Editor;
		e->setCaption("cedit");
		if (i > 0)
			e->loadFile(argv[i]);
		e->resize(900, 700);
		e->show();
	}
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
