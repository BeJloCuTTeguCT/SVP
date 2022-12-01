#ifndef DATABASE_H
#define DATABASE_H

#include <QList>

class Database
{
    QList<QString> *_service;
    QList<QString> *_specialist;
    QList<int> *_cost;
    QList<int> *_time;
public:
    Database(QString path_database = "");
    ~Database();
    bool read_Database(QString path_database);
    void write_Database(QString path_database);
    QList<QString> get_serviceList();
    QList<QString> get_specialistList();
    QList<int> get_costList();
    QList<int> get_timeList();
    void set_serviceList(QList<QString> *serviceList);
    void set_specialistList(QList<QString> *specialistList);
    void set_costList(QList<int> *costList);
    void set_timeList(QList<int> *timeList);
};

#endif // DATABASE_H
