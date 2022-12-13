#include "database.h"

#define F_SERVICE "название услуги"
#define F_SPECIALIST "название специалиста"
#define F_COST "стоимость услуг"
#define F_TIME "время оказания услуг"

Database::Database(QString path_database)
{
    this->_service = new QList<QString>;
    this->_specialist = new QList<QString>;
    this->_cost = new QList<int>;
    this->_time = new QList<int>;
    this->_service->clear();
    this->_specialist->clear();
    this->_cost->clear();
    this->_time->clear();
    if(path_database != "")
        this->read_Database(path_database);
}

Database::~Database()
{
    delete this->_service;
    delete this->_specialist;
    delete this->_cost;
    delete this->_time;
}

bool Database::read_Database(QString path_database)
{
    this->_service->clear();
    this->_specialist->clear();
    this->_cost->clear();
    this->_time->clear();
    QString val;
    QFile file;
    file.setFileName(path_database);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen())
        return false;
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
        this->_service->push_back(jObj1.value(F_SERVICE).toString());
        this->_specialist->push_back(jObj1.value(F_SPECIALIST).toString());
        this->_cost->push_back(jObj1.value(F_COST).toInt());
        this->_time->push_back(jObj1.value(F_TIME).toInt());
    }
    return true;
}

void Database::write_Database(QString path_database)
{
    QJsonArray jArr;
    for(int i = 0; i < this->_service->size(); ++i)
    {
        QJsonObject jObj;
        jObj.insert(F_SERVICE, this->_service->value(i));
        jObj.insert(F_SPECIALIST, this->_specialist->value(i));
        jObj.insert(F_COST, this->_cost->value(i));
        jObj.insert(F_TIME, this->_time->value(i));
        jArr.insert(i, jObj);
    }
    QJsonDocument JDoc;
    JDoc.setArray(jArr);
    QFile file(path_database);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(JDoc.toJson());
    file.close();
}

QList<QString> Database::get_serviceList()
{
    return *(this->_service);
}

QList<QString> Database::get_specialistList()
{
    return *(this->_specialist);
}

QList<int> Database::get_costList()
{
    return *(this->_cost);
}
QList<int> Database::get_timeList()
{
    return *(this->_time);
}

void Database::set_serviceList(QList<QString> *serviceList)
{
    this->_service->clear();
    this->_service->squeeze();
    *(this->_service) = *(serviceList);
}

void Database::set_specialistList(QList<QString> *specialistList)
{
    this->_specialist->clear();
    this->_specialist->squeeze();
    *(this->_specialist) = *(specialistList);
}

void Database::set_costList(QList<int> *costList)
{
    this->_cost->clear();
    this->_cost->squeeze();
    *(this->_cost) = *(costList);
}

void Database::set_timeList(QList<int> *timeList)
{
    this->_time->clear();
    this->_time->squeeze();
    *(this->_time) = *(timeList);
}
