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
//    PersonInfo seachInfo(QString key, PersonInfo& person);
    int insertPerson(PersonInfo person);
    int removePerson(QString key);
    int updateInfo(PersonInfo person);
    bool checkDuplicate(QString key);

//    void updateDB();
    QJsonDocument exportIDs();
    void test();

//signals:
//    void publishListID(QByteArray ListIDarr);
//    void ListIDtoPub(QJsonDocument jsonDoc);

private:
    QString dbPath;
    QString dbTableName;
    QSqlDatabase db;

//private:
//    QVector<PersonInfo> added_vec;
//    QVector<json> addedFail_jsonVec;
//    QVector<PersonInfo> addedFail_PersonVec;
//    QVector<QString> removed_vec;
//    QVector<json> removedFail_vec;
//    QVector<QString> removedFail_StringVec;
//    QVector<PersonInfo> updated_vec;
//    QVector<json> updatedFail_vec;
//    QVector<PersonInfo> updatedFail_PersonInfoVec;

};

#endif // SQLITEDB_H
