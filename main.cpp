#include <QCoreApplication>
#include <glog/logging.h>
#include "sqlitedb.h"
#include "mqttClientUpdate.h"
//#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MqttClientUpdate mqttClient;



    // =========================
    ///< TEST sqlideDB:
#if 0
    SQLiteDB sqliteDB;

    sqliteDB.setDBPath("cts_client.db");
    sqliteDB.setDBTable("CTS");
    sqliteDB.openDatabase();
    sqliteDB.exportIDs();
//    sqliteDB.removePerson("CMCTS-NGUYENTHITHU-0567");
#endif

    return a.exec();
}
