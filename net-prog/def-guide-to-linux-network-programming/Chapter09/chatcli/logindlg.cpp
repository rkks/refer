/* logindlg.cpp */
#include <qvbox.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include "logindlg.h"

cLoginDlg::cLoginDlg(QWidget *parent) : QDialog(parent, "", true)
{
    this->setCaption("Connect");
    this->setMinimumHeight(110);
    this->setMinimumWidth(200);
    
    QVBox *main = new QVBox(this);
    main->setSpacing(3);
    main->setMargin(3);
    main->setMinimumHeight(104);
    main->setMinimumWidth(200);
    
    QHBox *row1 = new QHBox(main);
    row1->setSpacing(3);
    QLabel *hostLabel = new QLabel("Host", row1);
    hostEdit = new QLineEdit(row1);

    QHBox *row2 = new QHBox(main);
    row2->setSpacing(3);
    QLabel *portLabel = new QLabel("Port", row2);
    portEdit = new QLineEdit(row2);

    QHBox *row3 = new QHBox(main);
    row3->setSpacing(3);
    QLabel *nickLabel = new QLabel("Nickname", row3);
    nickEdit = new QLineEdit(row3);

    QHBox *row4 = new QHBox(main);
    row4->setSpacing(10);
    QPushButton *connButton = new QPushButton("Connect", row4);
    this->connect(connButton, SIGNAL(clicked()), this, SLOT(accept()));
    QPushButton *cancelButton = new QPushButton("Cancel", row4);
    this->connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

cLoginDlg::~cLoginDlg()
{
}
