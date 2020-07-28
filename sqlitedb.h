#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QDateTime>

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
//#include <QTableView>
#include <QStringListModel>
#include <QSqlField>
#include <QSqlRecord>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#include "3rd/json.hpp"
#include <fstream>
#include "common.h"

using json = nlohmann::json;

class SQLiteDB: public QObject
{
    Q_OBJECT
public:
    explicit SQLiteDB(QObject *parent = nullptr);
    virtual ~SQLiteDB();

    void setDBPath(QString DBpath);
    void setDBTable(QString DBTableName);

    void initDatabase();
    void openDatabase();
    void closeDatabase();
    int insertPerson(PersonInfo person);
    int removePerson(QString key);
    int updateInfo(PersonInfo person);
    bool checkDuplicate(QString key);

//    void updateDB();
    QJsonDocument exportIDs();
    void test();

private:
    QString dbPath;
    QString dbTableName;
    QSqlDatabase db;

};

#endif // SQLITEDB_H
