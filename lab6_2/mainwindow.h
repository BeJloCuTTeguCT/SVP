#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
    QHBoxLayout *phbxlayout;
    QPushButton *pphbtn1;
    QPushButton *pphbtn2;
    QPushButton *pphbtn3;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
