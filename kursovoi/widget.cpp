#include "widget.h"
#include "database.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), _path_to_database(":/database/database"),
      _nameColums({"Услуга", "Специалист", "Стоимость", "Время оказания услуги"}), _changed(false),
      _str_filter(""), _index_filter(-1)
{
    this->_service = new QList<QString>;
    this->_specialist = new QList<QString>;
    this->_cost = new QList<int>;
    this->_time = new QList<int>;

    QVBoxLayout *pvbxlayout = new QVBoxLayout(this);
    QHBoxLayout *phbxlayotMenu = new QHBoxLayout;
    QHBoxLayout *phbxlayout1 = new QHBoxLayout;
    QHBoxLayout *phbxlayout2 = new QHBoxLayout;
    QVBoxLayout *pvbxlayout3 = new QVBoxLayout;
    QComboBox *pcmbbox_find = new QComboBox;
    QComboBox *pcmbbox_find_filter = new QComboBox;
    QComboBox *pcmbbox_filter = new QComboBox;
    QRadioButton *prdbtnV = new QRadioButton("По возрастанию");
    QRadioButton *prdbtnU = new QRadioButton("По убыванию");
    QMenuBar *pmenuBar = new QMenuBar(this);
    this->_ptable = new QTableWidget;

    prdbtnV->setChecked(true);

    pvbxlayout->addLayout(phbxlayotMenu);
    pvbxlayout->addLayout(phbxlayout1);
    pvbxlayout->addLayout(phbxlayout2);
    pvbxlayout->setSpacing(5);
    pvbxlayout->setContentsMargins(0, 0, 0, 0);
    phbxlayotMenu->addWidget(pmenuBar);
    phbxlayout1->addWidget(pcmbbox_find);
    phbxlayout1->addWidget(pcmbbox_find_filter);
    phbxlayout1->addStretch(2);
    phbxlayout1->addWidget(pcmbbox_filter);
    phbxlayout1->addLayout(pvbxlayout3);
    phbxlayout1->setSpacing(5);
    phbxlayout1->setContentsMargins(5, 5, 5, 5);
    phbxlayout2->setSpacing(5);
    phbxlayout2->setContentsMargins(5, 5, 5, 5);
    phbxlayout2->addWidget(this->_ptable);
    pvbxlayout3->addWidget(prdbtnV);
    pvbxlayout3->addWidget(prdbtnU);

    pcmbbox_find->setEditable(true);
    pcmbbox_find->lineEdit()->setPlaceholderText("Поиск");
    pcmbbox_find->setMinimumWidth(150);
    connect(pcmbbox_find, &QComboBox::textActivated, this, &Widget::onClick_find);
    pcmbbox_find_filter->setPlaceholderText("Категория поиска");
    pcmbbox_find_filter->addItems(this->_nameColums);
    connect(pcmbbox_find_filter, &QComboBox::activated, this, &Widget::onClick_find_filter);
    pcmbbox_filter->setPlaceholderText("Сортировка");
    pcmbbox_filter->addItems(this->_nameColums);

    QMenu *pmenuFile = new QMenu(tr("&Файл"), pmenuBar);
    QMenu *pmenuHelp = new QMenu(tr("&Справка"), pmenuBar);
    QAction *pOpenBD = pmenuFile->addAction(tr("Открыть &БД услуг"));
    pOpenBD->setShortcut(Qt::CTRL | Qt::Key_O);
    pmenuFile->addSeparator();
    QAction *pEditModeAction = pmenuFile->addAction(QPixmap(":/images/editICO"), "&Режим Редактирования");
    pEditModeAction->setShortcut(Qt::CTRL | Qt::Key_E);
    pEditModeAction->setCheckable(true);
    pEditModeAction->setChecked(false);
    QAction *pAddModeAction = pmenuFile->addAction(QPixmap(":/images/addICO"), "&Добавить запись");
    pAddModeAction->setShortcut(Qt::CTRL | Qt::Key_N);
    pRmModeAction = pmenuFile->addAction(QPixmap(":/images/removeICO"), "&Удалить запись");
    pRmModeAction->setShortcut(Qt::Key_Delete);
    pRmModeAction->setCheckable(true);
    pRmModeAction->setChecked(false);
    pmenuFile->addSeparator();
    QAction *pUpdMenuAction = pmenuFile->addAction("&Обновить таблицу");
    pSaveModeAction = pmenuFile->addAction(QPixmap(":/images/saveICO"), "Со&хранить измения");
    pSaveModeAction->setShortcut(Qt::CTRL | Qt::Key_S);
    pSaveModeAction->setDisabled(true);
    pClModeAction = pmenuFile->addAction(QPixmap(":/images/resetICO"), "Отменить и&змения");
    pClModeAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    pClModeAction->setDisabled(true);
    connect(pUpdMenuAction, &QAction::triggered, this, &Widget::loadTable);
    connect(pOpenBD, &QAction::triggered, this, &Widget::onClickedMenuOpenBD);
    connect(pEditModeAction, &QAction::triggered, this, &Widget::onClickedMenuRedact);
    connect(pAddModeAction, &QAction::triggered, this, &Widget::onClickedMenuAdd);
    connect(pRmModeAction, &QAction::triggered, this, &Widget::onClickedMenuRm);
    connect(pSaveModeAction, &QAction::triggered, this, &Widget::onClickedMenuSave);
    connect(pClModeAction, &QAction::triggered,this, &Widget::onClickedMenuCl);
    pmenuHelp->addAction(tr("О &Программе"));
    pmenuBar->addMenu(pmenuFile);
    pmenuBar->addMenu(pmenuHelp);

    this->loadTable();

    this->setLayout(pvbxlayout);
    this->resize(700, 700);
}

void Widget::onClick_find(const QString &str_var)
{
    this->_str_filter = str_var;
    this->find();
}

void Widget::onClick_find_filter(int index)
{
    this->_index_filter = index;
    this->find();
}

Widget::~Widget()
{
    delete this->_service;
    delete this->_specialist;
    delete this->_cost;
    delete this->_time;
}

void Widget::onClickedMenuOpenBD()
{
    this->_wgtPath = new Widget_GetPath;
    connect(_wgtPath, &Widget_GetPath::onClickedOkBtn, this, &Widget::getPathToDB);
}

void Widget::getPathToDB()
{
    this->_path_to_database = this->_wgtPath->get_pathToDB();
    this->loadTable();
    delete this->_wgtPath;
}

void Widget::onClickedMenuRedact(bool checked)
{
    if(checked)
        this->_ptable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    if(!checked)
        this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Widget::onClickedMenuAdd()
{
    this->_ptable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    this->_ptable->setRowCount(this->_ptable->rowCount()+1);
    this->_ptable->setCurrentCell(this->_ptable->rowCount()-1, 0);
    this->_changed = true;
    this->pSaveModeAction->setEnabled(true);
    this->pClModeAction->setEnabled(true);
}

void Widget::onClickedMenuRm(bool checked)
{
    if(checked)
    {
        this->_ptable->setColumnCount(this->_ptable->columnCount()+1);
        this->_ptable->setHorizontalHeaderLabels(QStringList() << this->_nameColums << "Удалить запись");
        for(int i = 0; i < this->_ptable->rowCount(); ++i)
        {
            QPushButton *phbtnRm = new QPushButton(QPixmap(":/images/removeICO"), "");
            this->_ptable->setIndexWidget(this->_ptable->model()->index(i, this->_ptable->columnCount()-1), phbtnRm);
            connect(phbtnRm, &QPushButton::clicked, this, &Widget::onClickedBtnRm);
        }
        this->_ptable->resizeColumnToContents(_ptable->columnCount()-1);
        return;
    }
    if (!checked)
    {
        this->_ptable->setColumnCount(this->_ptable->columnCount()-1);
        return;
    }
}

void Widget::onClickedBtnRm()
{
    this->_ptable->removeRow(this->_ptable->currentRow());
    this->_changed = true;
    this->pSaveModeAction->setEnabled(true);
    this->pClModeAction->setEnabled(true);
}

void Widget::onClickedMenuSave()
{
    this->_service->clear();
    this->_service->squeeze();
    this->_specialist->clear();
    this->_specialist->squeeze();
    this->_cost->clear();
    this->_cost->squeeze();
    this->_time->clear();
    this->_time->squeeze();
    int row = this->_ptable->rowCount();
    for(int i = 0; i < row; ++i)
        this->_service->append(this->_ptable->takeItem(i, 0)->text());
    for(int i = 0; i < row; ++i)
        this->_specialist->append(this->_ptable->takeItem(i, 1)->text());
    for(int i = 0; i < row; ++i)
        this->_cost->append(this->_ptable->takeItem(i, 2)->text().toInt());
    for(int i = 0; i < row; ++i)
        this->_time->append(this->_ptable->takeItem(i, 3)->text().toInt());

    this->_changed = false;
    this->pSaveModeAction->setDisabled(true);
    this->pClModeAction->setDisabled(true);
    this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(pRmModeAction->isChecked())
    {
        pRmModeAction->setChecked(false);
        this->onClickedMenuRm(false);
    }

    this->saveData();
}

void Widget::onClickedMenuCl()
{
    this->_changed = false;
    this->pSaveModeAction->setDisabled(true);
    this->pClModeAction->setDisabled(true);
    this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(pRmModeAction->isChecked())
    {
        pRmModeAction->setChecked(false);
        this->onClickedMenuRm(false);
    }

    this->reloadTable();
}

void Widget::onClickedRedactRow()
{
    int selectRow = this->_ptable->currentRow();
    if(this->_ptable->item(selectRow, 0)->flags().testFlag(Qt::ItemIsSelectable))
    {
        this->_ptable->setEditTriggers(QAbstractItemView::AllEditTriggers);
        this->_ptable->setSelectionBehavior(QAbstractItemView::SelectItems);
        this->_ptable->setSelectionMode(QAbstractItemView::SingleSelection);
        for(int i = 0; i < this->_ptable->columnCount()-1; ++i)
        {
            QList<QString> data({_service->value(selectRow), _specialist->value(selectRow), QString::number(_cost->value(selectRow)), QString::number(_time->value(selectRow))});
            QTableWidgetItem *item = this->_ptable->item(selectRow, i);
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            item->setText(data.value(i));
        }
        return;
    }
    if(this->_ptable->item(selectRow, 0)->flags().testFlag(Qt::ItemIsEditable))
    {
        for(int i = 0; i < this->_ptable->columnCount()- 1; ++i)
        {
            QTableWidgetItem *item(this->_ptable->takeItem(selectRow, i));
            item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
        }
        this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->_ptable->setSelectionBehavior(QAbstractItemView::SelectRows);
        this->_ptable->setSelectionMode(QAbstractItemView::SingleSelection);
        return;
    }
}

void Widget::onClickedRedactSave()
{
    this->_service->clear();
    this->_service->squeeze();
    this->_specialist->clear();
    this->_specialist->squeeze();
    this->_cost->clear();
    this->_cost->squeeze();
    this->_time->clear();
    this->_time->squeeze();
    for(int i = 0; i < this->_service->size(); ++i)
        this->_service->append(this->_ptable->item(i, 0)->text());
    for(int i = 0; i < this->_specialist->size(); ++i)
        this->_specialist->append(this->_ptable->item(i, 1)->text());
    for(int i = 0; i < this->_cost->size(); ++i)
         this->_cost->append(this->_ptable->item(i, 2)->text().toInt());
    for(int i = 0; i < this->_time->size(); ++i)
        this->_time->append(this->_ptable->item(i, 3)->text().toInt());
    this->saveData();
}

void Widget::onClickedRedactCancel()
{
    this->reloadTable();
}

void Widget::getData()
{
    this->_service->clear();
    this->_service->squeeze();
    this->_specialist->clear();
    this->_specialist->squeeze();
    this->_cost->clear();
    this->_cost->squeeze();
    this->_time->clear();
    this->_time->squeeze();
    Database *Db = new Database(this->_path_to_database);
    *(this->_service) = Db->get_serviceList();
    *(this->_specialist) = Db->get_specialistList();
    *(this->_cost) = Db->get_costList();
    *(this->_time) = Db->get_timeList();
    delete Db;
}

void Widget::saveData()
{
    Database *Db = new Database;
    Db->set_serviceList(this->_service);
    Db->set_specialistList(this->_specialist);
    Db->set_costList(this->_cost);
    Db->set_timeList(this->_time);
    Db->write_Database(this->_path_to_database);
    delete Db;
}

void Widget::loadTable()
{
    this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->_ptable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->_ptable->verticalHeader()->setVisible(false);
    this->_ptable->setColumnCount(this->_nameColums.size());
    this->_ptable->setHorizontalHeaderLabels(this->_nameColums);

    this->getData();
    this->_ptable->setRowCount(this->_service->size());
    for(int i = 0; i < this->_service->size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(this->_service->value(i));
        this->_ptable->setItem(i, 0, item);
    }
    for(int i = 0; i < this->_specialist->size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(this->_specialist->value(i));
        this->_ptable->setItem(i, 1, item);
    }
    for(int i = 0; i < this->_cost->size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(this->_cost->value(i)));

        this->_ptable->setItem(i, 2, item);
//        item->setFlags(item->flags() ^ Qt::ItemIsSelectable);

    }
    for(int i = 0; i < this->_time->size(); ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(this->_time->value(i)));
        this->_ptable->setItem(i, 3, item);
    }

    this->_ptable->resizeColumnsToContents();
}

void Widget::reloadTable()
{
    this->_ptable->setRowCount(this->_service->size());
    for(int i = 0; i < this->_service->size(); ++i)
        this->_ptable->setItem(i, 0, new QTableWidgetItem(this->_service->value(i)));
    for(int i = 0; i < this->_specialist->size(); ++i)
        this->_ptable->setItem(i, 1, new QTableWidgetItem(this->_specialist->value(i)));
    for(int i = 0; i < this->_cost->size(); ++i)
        this->_ptable->setItem(i, 2, new QTableWidgetItem(QString::number(this->_cost->value(i))));
    for(int i = 0; i < this->_time->size(); ++i)
        this->_ptable->setItem(i, 3, new QTableWidgetItem(QString::number(this->_time->value(i))));

    this->_ptable->resizeColumnsToContents();
}

void Widget::tableToFind(QList<int> indexs)
{
    int rowCount = indexs.size();
    if(rowCount > 0)
    {
        while(this->_ptable->rowCount() > rowCount)
            this->_ptable->removeRow(this->_ptable->rowCount()-1);
        foreach (int col, indexs) {
            rowCount--;
            QTableWidgetItem *item = new QTableWidgetItem(this->_service->value(col));
            this->_ptable->setItem(rowCount, 0, item);
            item = new QTableWidgetItem(this->_specialist->value(col));
            this->_ptable->setItem(rowCount, 1, item);
            item = new QTableWidgetItem(QString::number(this->_cost->value(col)));
            this->_ptable->setItem(rowCount, 2, item);
            item = new QTableWidgetItem(QString::number(this->_time->value(col)));
            this->_ptable->setItem(rowCount, 3, item);
        }
    }
    else
        this->reloadTable();
}

void Widget::find()
{
    if(!(this->_str_filter.isEmpty()) && this->_index_filter >= 0)
    {
        QList<int> indexs;
        switch (this->_index_filter)
        {
        case 0: {
            for(int i = 0; i < this->_service->size(); ++i) {
                QString col = this->_service->value(i);
                if(col == this->_str_filter)
                    indexs.append(i);
            }
            break;
        }
        case 1: {
            for(int i = 0; i < this->_specialist->size(); ++i) {
                QString col = this->_specialist->value(i);
                if(col == this->_str_filter)
                    indexs.append(i);
            }
            break;
        }
        case 2:
        {
            for(int i = 0; i < this->_cost->size(); ++i) {
                QString col = QString::number(this->_cost->value(i));
                if(col == this->_str_filter)
                    indexs.append(i);
            }
            break;
        }
        case 3:
        {
            for(int i = 0; i < this->_time->size(); ++i) {
                QString col = QString::number(this->_time->value(i));
                if(col == this->_str_filter)
                    indexs.append(i);
            }
            break;
        }
        default:
            return;
        }
        this->tableToFind(indexs);
    }
}

Widget_GetPath::Widget_GetPath(QWidget *parent)
    : QWidget(parent), _path_to_database(":/database/database")
{
    pWgtEditPathDB = new QWidget;
    QHBoxLayout *playout = new QHBoxLayout;
    pLineEditPathDB = new QLineEdit;
    QPushButton *pphbtnOk = new QPushButton("Ok");

    pLineEditPathDB->setPlaceholderText("Название файла *.json с путем до него");
    connect(pphbtnOk, &QPushButton::clicked, this, &Widget_GetPath::onClickedOkBtn);
    playout->addWidget(pLineEditPathDB);
    playout->addWidget(pphbtnOk);
    pWgtEditPathDB->setLayout(playout);
    pWgtEditPathDB->setMinimumWidth(350);
    pWgtEditPathDB->show();
}

Widget_GetPath::~Widget_GetPath()
{
    delete pWgtEditPathDB;
}

QString Widget_GetPath::get_pathToDB()
{
    return this->pLineEditPathDB->text();
}

