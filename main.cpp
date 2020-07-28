#include <QCoreApplication>
#include <glog/logging.h>
#include "sqlitedb.h"
#include "mqttClientUpdate.h"
//#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MqttClientUpdate mqttClient;

    return a.exec();
}
