#include "widget.h"
#include "database.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), _path_to_database(":/database/database"),
      _nameColums({"Услуга", "Специалист", "Стоимость", "Время оказания услуги"}), _changed(false),
      _sortParam(false), _indexSort(-1), _str_filter(""), _index_filter(-1)
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
    QMenuBar *pmenuBar = new QMenuBar(this);
    this->_prdbtnV = new QRadioButton("По возрастанию");
    this->_prdbtnU = new QRadioButton("По убыванию");
    this->_ptable = new QTableWidget;

    this->_prdbtnV->setChecked(true);
    connect(this->_prdbtnV, &QRadioButton::clicked, this, &Widget::onClickedSortBtnV);
    connect(this->_prdbtnU, &QRadioButton::clicked, this, &Widget::onClickedSortBtnU);

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
    pvbxlayout3->addWidget(this->_prdbtnV);
    pvbxlayout3->addWidget(this->_prdbtnU);

    pcmbbox_find->setEditable(true);
    pcmbbox_find->lineEdit()->setPlaceholderText("Поиск");
    pcmbbox_find->setMinimumWidth(150);
    connect(pcmbbox_find, &QComboBox::textActivated, this, &Widget::onClick_find);
    pcmbbox_find_filter->setPlaceholderText("Категория поиска");
    pcmbbox_find_filter->addItems(this->_nameColums);
    connect(pcmbbox_find_filter, &QComboBox::activated, this, &Widget::onClick_find_filter);
    pcmbbox_filter->setPlaceholderText("Сортировка");
    pcmbbox_filter->addItems(this->_nameColums);
    connect(pcmbbox_filter, &QComboBox::activated, this, &Widget::sort);

    QMenu *pmenuFile = new QMenu("&Файл", pmenuBar);
    QMenu *pmenuHelp = new QMenu("&Справка", pmenuBar);
    QAction *pOpenBD = pmenuFile->addAction("Открыть &БД услуг");
    pOpenBD->setShortcut(Qt::CTRL | Qt::Key_O);
    pmenuFile->addSeparator();
    pEditModeAction = pmenuFile->addAction(QPixmap(":/images/editICO"), "&Режим Редактирования");
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
    pUpdMenuAction->setShortcut(Qt::Key_F5);
    pSaveModeAction = pmenuFile->addAction(QPixmap(":/images/saveICO"), "Со&хранить измения");
    pSaveModeAction->setShortcut(Qt::CTRL | Qt::Key_S);
    pSaveModeAction->setDisabled(true);
    pClModeAction = pmenuFile->addAction(QPixmap(":/images/resetICO"), "Отменить и&змения");
    pClModeAction->setShortcut(Qt::CTRL | Qt::Key_Z);
    pClModeAction->setDisabled(true);
    QAction *pProgAction = pmenuHelp->addAction(tr("О &Программе"));
    connect(pUpdMenuAction, &QAction::triggered, this, &Widget::loadTable);
    connect(pOpenBD, &QAction::triggered, this, &Widget::onClickedMenuOpenBD);
    connect(pEditModeAction, &QAction::triggered, this, &Widget::onClickedMenuRedact);
    connect(pAddModeAction, &QAction::triggered, this, &Widget::onClickedMenuAdd);
    connect(pRmModeAction, &QAction::triggered, this, &Widget::onClickedMenuRm);
    connect(pSaveModeAction, &QAction::triggered, this, &Widget::onClickedMenuSave);
    connect(pClModeAction, &QAction::triggered,this, &Widget::onClickedMenuCl);
    connect(pProgAction, &QAction::triggered, this, &Widget::onClickedOProgramme);

    pmenuBar->addMenu(pmenuFile);
    pmenuBar->addMenu(pmenuHelp);

    this->loadTable();

    this->_path_to_database = QDir::currentPath() + "\\database.json";
    QFile file(_path_to_database);
    file.open(QIODevice::WriteOnly);
    file.close();
    this->saveData();

    this->setWindowTitle("Список медицинских услуг");
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

void Widget::onClickedSortBtnV()
{
    this->_sortParam = false;
    this->sortFunc();
}

void Widget::onClickedSortBtnU()
{
    this->_sortParam = true;
    this->sortFunc();
}

void Widget::sort(int index)
{
    this->_indexSort = index;
    this->sortFunc();
}

void Widget::onClickedOProgramme()
{
    this->_pWgtOProgramme = new QDialog;
    QHBoxLayout *pMainlayout = new QHBoxLayout;
    QVBoxLayout *pLayout = new QVBoxLayout;
    QHBoxLayout *pLayoutBtn = new QHBoxLayout;
    QLabel *pLabel1 = new QLabel("Список медицинских услуг v0.0.1");
    QLabel *pLabel2 = new QLabel("Программа собрана 06.12.2022");
    QLabel *pLabel3 = new QLabel("Основана на Qt 6.2.2 (MinGW 9.0, 64 бита)");
    QPushButton *pPhBtn = new QPushButton("Закрыть");
    _pWgtOProgramme->setWindowTitle("О Программе список медицинских услуг");
    _pWgtOProgramme->setFixedSize(350, 200);
    _pWgtOProgramme->setLayout(pMainlayout);
    pMainlayout->addLayout(pLayout);
    pLayout->addWidget(pLabel1);
    pLayout->addWidget(pLabel2);
    pLayout->addWidget(pLabel3);
    pLayout->addLayout(pLayoutBtn);
    pLabel1->setStyleSheet("font-weight: bold");
    pLayoutBtn->addStretch(1);
    pLayoutBtn->addWidget(pPhBtn);
    connect(pPhBtn, &QPushButton::clicked, this, &Widget::onClickedCloseOProgramme);
    _pWgtOProgramme->show();
}

void Widget::onClickedCloseOProgramme()
{
    delete this->_pWgtOProgramme;
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
    {
        this->_changed = true;
        this->pSaveModeAction->setEnabled(true);
        this->pClModeAction->setEnabled(true);
        this->_ptable->setEditTriggers(QAbstractItemView::AllEditTriggers);
        this->_ptable->verticalHeader()->setVisible(true);
    }
    if(!checked)
    {
        this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->_ptable->verticalHeader()->setVisible(false);
    }
}

void Widget::onClickedMenuAdd()
{
    this->_ptable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    this->_ptable->setRowCount(this->_ptable->rowCount()+1);
    this->_ptable->setCurrentCell(this->_ptable->rowCount()-1, 0);
    this->_ptable->verticalHeader()->setVisible(true);
    for(int i = 0; i < this->_nameColums.size(); ++i)
        this->_ptable->setItem(this->_ptable->rowCount()-1, i, new QTableWidgetItem(""));
    if(pRmModeAction->isChecked())
    {
        QPushButton *phbtnRm = new QPushButton(QPixmap(":/images/removeICO"), "");
        this->_ptable->setIndexWidget(this->_ptable->model()->index(this->_ptable->rowCount()-1, this->_ptable->columnCount()-1), phbtnRm);
        connect(phbtnRm, &QPushButton::clicked, this, &Widget::onClickedBtnRm);
    }
    this->_changed = true;
    this->pSaveModeAction->setEnabled(true);
    this->pClModeAction->setEnabled(true);
}

void Widget::onClickedMenuRm(bool checked)
{
    if(checked)
    {
        this->_ptable->verticalHeader()->setVisible(true);
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
        this->_ptable->verticalHeader()->setVisible(false);
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
    QList<QPoint> vErrorsItemsPts;
    for(int i = 0; i < row; ++i)
    {

        QString tempString = this->_ptable->itemFromIndex(this->_ptable->model()->index(i, 0))->text();
        if(!tempString.isEmpty())
            this->_service->append(tempString);
        else
            vErrorsItemsPts.append(QPoint(0, i));
        tempString = this->_ptable->itemFromIndex(this->_ptable->model()->index(i, 1))->text();
        if(!tempString.isEmpty())
            this->_specialist->append(tempString);
        else
            vErrorsItemsPts.append(QPoint(1, i));
    }
    for(int i = 0; i < row; ++i)
    {
        bool isInt7;
        int tempInt =-1;
        tempInt = this->_ptable->itemFromIndex(this->_ptable->model()->index(i, 2))->text().toInt(&isInt7, 10);
        if(isInt7 && tempInt >=0)
            this->_cost->append(tempInt);
        else
            vErrorsItemsPts.append(QPoint(2, i));
        tempInt = this->_ptable->itemFromIndex(this->_ptable->model()->index(i, 3))->text().toInt(&isInt7, 10);
        if(isInt7 && tempInt >=0)
            this->_time->append(tempInt);
        else
            vErrorsItemsPts.append(QPoint(3, i));

    }

    if(vErrorsItemsPts.size() != 0 )
    {
        QString textForErrorMess("Ошибка формата данных - требуются символы в первых двух столбцах,"
                                 "\nне отрицательные цифры в двух последних! Ошибка");
        for (int i = 0; i < vErrorsItemsPts.size(); ++i)
            textForErrorMess.append("\nв столбце " + this->_nameColums.value(vErrorsItemsPts.value(i).x()) + " и "
                                    + QString::number(vErrorsItemsPts.value(i).y() + 1) + " строке");
        if(vErrorsItemsPts.size() < 45)
            QMessageBox::critical(this, "Список медицинских услуг",
                                        textForErrorMess,
                                        QMessageBox::Ok, QMessageBox::Ok);
        else
            QMessageBox::critical(this, "Список медицинских услуг",
                                  "Ошибка формата данных - требуются символы в первых двух столбцах,"
                                  "\nне отрицательные цифры в двух последних! Количество ошибок - более 44",
                                        QMessageBox::Ok, QMessageBox::Ok);
        this->getData();
        return;
    }

    this->_changed = false;
    this->pSaveModeAction->setDisabled(true);
    this->pClModeAction->setDisabled(true);
    this->pEditModeAction->setChecked(false);
    this->_ptable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->_ptable->verticalHeader()->setVisible(false);

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

    this->_ptable->verticalHeader()->setVisible(false);
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

void Widget::sortFunc()
{
    int index = this->_indexSort;
        if(!this->_sortParam) //сортировка по возрастанию
        {
            switch (index)
            {
            case 0:
            {
                QList<QString> temp = *(this->_service);
                QList<int> temp2 = *(this->_cost);
                QList<int> temp_index;
                std::sort(this->_service->begin(), this->_service->end(), [] (QString &v1, QString &v2) {return v1 < v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_service->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_specialist->append(temp.value(temp_index.value(i)));
                this->_cost->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_cost->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_time->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 1:
            {
                QList<QString> temp = *(this->_specialist);
                QList<int> temp2 = *(this->_cost);
                QList<int> temp_index;
                std::sort(this->_specialist->begin(), this->_specialist->end(), [] (QString &v1, QString &v2) {return v1 < v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_specialist->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_service);
                this->_service->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_service->append(temp.value(temp_index.value(i)));
                this->_cost->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_cost->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_time->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 2:
            {
                QList<int> temp = *(this->_cost);
                QList<QString> temp2 = *(this->_service);
                QList<int> temp_index;
                std::sort(this->_cost->begin(), this->_cost->end(), [] (int &v1, int &v2) {return v1 < v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_cost->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_time->append(temp.value(temp_index.value(i)));
                this->_service->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_service->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_specialist->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 3:
            {
                QList<int> temp = *(this->_time);
                QList<QString> temp2 = *(this->_service);
                QList<int> temp_index;
                std::sort(this->_time->begin(), this->_time->end(), [] (int &v1, int &v2) {return v1 < v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_time->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_cost);
                this->_cost->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_cost->append(temp.value(temp_index.value(i)));
                this->_service->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_service->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_specialist->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            }
        }
        if(this->_sortParam) //сортировка по убыванию
        {
            switch (index)
            {
            case 0:
            {
                QList<QString> temp = *(this->_service);
                QList<int> temp2 = *(this->_cost);
                QList<int> temp_index;
                std::sort(this->_service->begin(), this->_service->end(), [] (QString &v1, QString &v2) {return v1 > v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_service->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_specialist->append(temp.value(temp_index.value(i)));
                this->_cost->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_cost->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_time->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 1:
            {
                QList<QString> temp = *(this->_specialist);
                QList<int> temp2 = *(this->_cost);
                QList<int> temp_index;
                std::sort(this->_specialist->begin(), this->_specialist->end(), [] (QString &v1, QString &v2) {return v1 > v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_specialist->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_service);
                this->_service->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_service->append(temp.value(temp_index.value(i)));
                this->_cost->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_cost->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_time->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 2:
            {
                QList<int> temp = *(this->_cost);
                QList<QString> temp2 = *(this->_service);
                QList<int> temp_index;
                std::sort(this->_cost->begin(), this->_cost->end(), [] (int &v1, int &v2) {return v1 > v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_cost->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_time);
                this->_time->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_time->append(temp.value(temp_index.value(i)));
                this->_service->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_service->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_specialist->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            case 3:
            {
                QList<int> temp = *(this->_time);
                QList<QString> temp2 = *(this->_service);
                QList<int> temp_index;
                std::sort(this->_time->begin(), this->_time->end(), [] (int &v1, int &v2) {return v1 > v2;});
                for(int i = 0; i < temp.size(); ++i)
                    for(int j = 0; j < temp.size(); ++j)
                        if(this->_time->value(i) == temp.value(j) && !temp_index.contains(j))
                        {
                            temp_index.append(j);
                            break;
                        }
                temp = *(this->_cost);
                this->_cost->clear();
                for(int i = 0; i < temp.size(); ++i)
                    this->_cost->append(temp.value(temp_index.value(i)));
                this->_service->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_service->append(temp2.value(temp_index.value(i)));
                temp2 = *(this->_specialist);
                this->_specialist->clear();
                for(int i = 0; i < temp2.size(); ++i)
                    this->_specialist->append(temp2.value(temp_index.value(i)));
                this->reloadTable();
                break;
            }
            }
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

