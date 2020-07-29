#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QDateTime>
#include <QImage>
#include <QSettings>

// -- Define ---
#define LOG_DEBUG
#define export_to_json

typedef struct{
    QString recognize_name;
    QString recognize_id;
    QString tracking_id;
    QDateTime updatedAt;

    QString fullName;
    QString department = "unknown";
    QString company = "unknown";
    QString level;
    QImage avt = QImage("files/unknown_person.png");
    quint8 ver =  1;
    QByteArray avtByteArray;
}PersonInfo;

#define MQTT_TOPIC_COMMON "toClient/#"
//#define MQTT_TOPIC_COMMON_RECEIVED
#define MQTT_SUBTOPIC_REQUEST_RECEIVED tr("toClient/GeneralExternalRequests")
#define MQTT_TOPIC_REQUEST_RESPONSE tr("toApp/DB_Status_Ack")
#define MQTT_SUBTOPIC_DBUPDATED_RECEIVED tr("toClient/DB_Data")
#define MQTT_TOPIC_UPDATED_STATUS_RESPONSE tr("toApp/Updated_Status")

//#define SQLITE_DBPATH
//#define SQLITE_DBPATH "/media/huynv/Data/02.Project_SmartCamera/08.GUI/build-FaceAppGui_v20-Desktop_Qt_5_12_6_GCC_64bit-Debug/db/cts_client.db"
//#define SQLITE_DBTABLE "CTS"


#endif // COMMON_H
