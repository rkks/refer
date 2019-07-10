/* chatwin.h */
#ifndef CHATWIN_H
#define CHATWIN_H

#include <qmainwindow.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qtimer.h>


class cChatWin : public QMainWindow
{
Q_OBJECT
public:
    cChatWin();
    ~cChatWin();

public:
    int client_socket;
    QTextEdit *chatEdit;
    QLineEdit *msgEdit;
    QListBox *userList;

protected slots:
    void sendButtonClicked();
    void privButtonClicked();
    void opButtonClicked();
    void kickButtonClicked();
    void topicButtonClicked();
    void quitButtonClicked();
    void timerFired();

protected:
    QTimer *theTimer;
};


#endif
