#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QMqttClient>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QVector>
#include <QThread>
#include <QQueue>
#include "3rd/json.hpp"
#include <fstream>

#include "sqlitedb.h"

using json = nlohmann::json;

class Update : public QObject
{
    Q_OBJECT
public:
    Update();
    int readJsonFromFile(QString fileName);

    int updateDB(QString dbPath, QString dbTableName);

//slots:
//    void exportIDList();

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

    QString dbPath;
    QString dbTableName;


};

#endif // UPDATE_H
