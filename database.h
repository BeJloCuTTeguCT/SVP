#ifndef DATABASE_H
#define DATABASE_H

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

struct Data_To_Row_In_List
{
    QString _service;
    QString _specialist;
    int _cost;
    int _time;
    Data_To_Row_In_List();
    Data_To_Row_In_List(const Data_To_Row_In_List &temp);
    Data_To_Row_In_List(QString service, QString specialict, int cost, int time);
};

class Database
{
    QList<Data_To_Row_In_List> *_RowsList;
public:
    Database(QString path_database = "");
    ~Database();
    bool read_Database(QString path_database);
    bool write_Database(QString path_database);
    QList<Data_To_Row_In_List> get_RowsList();
    void set_RowsList(QList<Data_To_Row_In_List> *rowsList);
};

#endif // DATABASE_H
