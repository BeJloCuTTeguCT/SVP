#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QHBoxLayout *phbxlayout = new QHBoxLayout;
    QPushButton *pphbtn1 = new QPushButton(tr("Show Error"));
    QPushButton *pphbtn2 = new QPushButton(tr("Show Watrning"));
    QPushButton *pphbtn3 = new QPushButton(tr("Show Information"));

    this->setLayout(phbxlayout);
    phbxlayout->setParent(this);
    phbxlayout->addWidget(pphbtn1);
    phbxlayout->addWidget(pphbtn2);
    phbxlayout->addWidget(pphbtn3);
}


MainWindow::~MainWindow()
{
}

