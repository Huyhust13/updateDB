#include <QCoreApplication>
#include <QSettings>
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
    utils.initLog(logDir);
    FLAGS_log_dir = logDir;
    google::InitGoogleLogging(argv[0]);
//    Log to Terminal: 0 - log to file, 1 - log to terminal
    QSettings settings("updateDB.conf", QSettings::IniFormat);
    FLAGS_logtostderr = settings.value("logToTerminal", false).toBool();;

    QCoreApplication a(argc, argv);

    MqttClientUpdate mqttClient;

    return a.exec();
}
