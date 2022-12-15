#include "database.h"

#define F_SERVICE "название услуги"
#define F_SPECIALIST "название специалиста"
#define F_COST "стоимость услуг"
#define F_TIME "время оказания услуг"

Data_To_Row_In_List::Data_To_Row_In_List():
    _service(""), _specialist(""), _cost(0), _time(0) {}

Data_To_Row_In_List::Data_To_Row_In_List(const Data_To_Row_In_List &temp) :
    _service(temp._service), _specialist(temp._specialist), _cost(temp._cost), _time(temp._time) {};

Data_To_Row_In_List::Data_To_Row_In_List(QString service, QString specialict, int cost, int time) :
    _service(service), _specialist(specialict), _cost(cost), _time(time) {};

Database::Database(QString path_database)
{
    this->_RowsList = new QList<Data_To_Row_In_List>;
    if(path_database != "")
        this->read_Database(path_database);
}

Database::~Database()
{
    delete this->_RowsList;
}

bool Database::read_Database(QString path_database)
{
    this->_RowsList->clear();
    QString val;
    QFile file(path_database);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        val = file.readAll();
        file.close();
        QJsonDocument JDoc = QJsonDocument::fromJson(val.toUtf8());
        QJsonArray jArr = JDoc.array();
        if(jArr.size() == 0)
            return false;
        for(int i = 0; i < jArr.size(); ++i)
        {
            QJsonObject jObj1;
            jObj1 = jArr.at(i).toObject();
            this->_RowsList->append(Data_To_Row_In_List(
                                        jObj1.value(F_SERVICE).toString(), jObj1.value(F_SPECIALIST).toString(),
                                        jObj1.value(F_COST).toInt(), jObj1.value(F_TIME).toInt()));
        }
        return true;
    }
    return false;
}

bool Database::write_Database(QString path_database)
{
    QJsonArray jArr;
    for(int i = 0; i < this->_RowsList->size(); ++i)
    {
        QJsonObject jObj;
        Data_To_Row_In_List *RowsList = new Data_To_Row_In_List(this->_RowsList->value(i));
        jObj.insert(F_SERVICE, RowsList->_service);
        jObj.insert(F_SPECIALIST, RowsList->_specialist);
        jObj.insert(F_COST, RowsList->_cost);
        jObj.insert(F_TIME, RowsList->_time);
        jArr.insert(i, jObj);
        delete RowsList;
    }
    QJsonDocument JDoc;
    JDoc.setArray(jArr);
    QFile file(path_database);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(JDoc.toJson());
        file.close();
        return true;
    }
    return false;
}

QList<Data_To_Row_In_List> Database::get_RowsList()
{
    return *(this->_RowsList);
}

void Database::set_RowsList(QList<Data_To_Row_In_List> *rowsList)
{
    this->_RowsList->clear();
    this->_RowsList->squeeze();
    *(this->_RowsList) = *(rowsList);
}


