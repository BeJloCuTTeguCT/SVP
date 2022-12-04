#ifndef WIDGET_H
#define WIDGET_H
#include <QDebug>

#include <algorithm>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTableWidget>
#include <QMenuBar>
#include <QHeaderView>
#include <QPixmap>
#include <QMessageBox>

class Widget_GetPath;

class Widget : public QWidget
{
    Q_OBJECT
    QString _path_to_database;
    Widget_GetPath *_wgtPath;
    QStringList _nameColums;
    QVBoxLayout *pvbxlayout;
    QHBoxLayout *phbxlayout1;
    QHBoxLayout *phbxlayout2;
    QAction *pRmModeAction;
    QAction *pSaveModeAction;
    QAction *pClModeAction;
    bool _changed;
    QRadioButton *_prdbtnV;
    QRadioButton *_prdbtnU;
    bool _sortParam;
    int _indexSort;
    QTableWidget *_ptable;
    QMenuBar *pmenuBar;
    QMenu *pmenuFile;
    QMenu *pmenuHelp;
    QList<QString> *_service;
    QList<QString> *_specialist;
    QList<int> *_cost;
    QList<int> *_time;
    QString _str_filter;
    int _index_filter;
    void getData();
    void saveData();
    void loadTable();
    void reloadTable();
    void tableToFind(QList<int> indexs);
    void find();
    void sortFunc();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected slots:
    void onClickedMenuOpenBD();
    void getPathToDB();
    void onClickedMenuRedact(bool checked);
    void onClickedMenuAdd();
    void onClickedMenuRm(bool checked);
    void onClickedBtnRm();
    void onClickedMenuSave();
    void onClickedMenuCl();
    void onClickedRedactRow();
    void onClickedRedactSave();
    void onClickedRedactCancel();
    void onClick_find(const QString &str);
    void onClick_find_filter(int index);
    void onClickedSortBtnV();
    void onClickedSortBtnU();
    void sort(int index);
};

class Widget_GetPath: public QWidget
{
    Q_OBJECT
    QString _path_to_database;
    QWidget *pWgtEditPathDB;
    QLineEdit *pLineEditPathDB;
public:
    Widget_GetPath(QWidget *parent = nullptr);
    ~Widget_GetPath();
    QString get_pathToDB();
signals:
    void onClickedOkBtn();
};

#endif // WIDGET_H
