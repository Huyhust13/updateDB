#include <QCoreApplication>
#include <glog/logging.h>
#include "sqlitedb.h"
#include "mqttClientUpdate.h"
#include "utils.h"
//#include <QObject>

int main(int argc, char *argv[])
{

//    Change Log Dir
    std::string logDir = "log";
    Utils utils = Utils();
    int mkdirLog = utils.initLog(logDir);
    FLAGS_log_dir = logDir;
    google::InitGoogleLogging(argv[0]);
//    Log to Terminal: 0 - log to file, 1 - log to terminal
    FLAGS_logtostderr = 1;

    QCoreApplication a(argc, argv);

    MqttClientUpdate mqttClient;

    return a.exec();
}
