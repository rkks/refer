/* logindlg.h */
#ifndef LOGINDLG_H
#define LOGINDLG_H


#include <qdialog.h>
#include <qlineedit.h>

class cLoginDlg : public QDialog
{
Q_OBJECT
public:
    cLoginDlg(QWidget *parent);
    ~cLoginDlg();

public:
    QLineEdit *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *nickEdit;
};


#endif
