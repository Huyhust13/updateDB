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
using json = nlohmann::json;

typedef struct{
    QString recognize_name;
    QString recognize_id;
//    qint8 tracking_id;
    QString tracking_id;
    QDateTime updatedAt;
    QImage detected_face;

    QString fullName;
    QString employeeID;
    QString department = "unknown";
    QString level;
    QImage avt = QImage("files/unknown_person.png");
    QByteArray avtByteArray;
    quint8 ver =  1;
}PersonInfo;

//#define DB_NAME "cts_client.db";
#define DB_NAME "/media/huynv/Data/02.Project_SmartCamera/08.GUI/build-FaceAppGui_v20-Desktop_Qt_5_12_6_GCC_64bit-Debug/db/cts_client.db";
#define TABLE_NAME "CTS";

class SQLiteDB: public QObject
{
    Q_OBJECT
public:
    explicit SQLiteDB(QObject *parent = nullptr);
    virtual ~SQLiteDB();

    void setDBName(QString DBpath);
    void setDBTable(QString DBTableName);

    void openDatabase();
//    PersonInfo seachInfo(QString key, PersonInfo& person);
    int insertPerson(PersonInfo person);
    int removePerson(QString key);
    int updateInfo(PersonInfo person);
    bool checkDuplicate(QString key);

//public slots:
    void updateDB();
    QJsonDocument exportIDs();

signals:
    void publishListID(QByteArray ListIDarr);
    void ListIDtoPub(QJsonDocument jsonDoc);

private:
    QString dbPath;
    QString dbTableName;

private:
    QVector<PersonInfo> added_vec;
    QVector<json> addedFail_jsonVec;
    QVector<PersonInfo> addedFail_PersonVec;
    QVector<QString> removed_vec;
    QVector<json> removedFail_vec;
    QVector<QString> removedFail_StringVec;
    QVector<PersonInfo> updated_vec;
    QVector<json> updatedFail_vec;
    QVector<PersonInfo> updatedFail_PersonInfoVec;

};

#endif // SQLITEDB_H
