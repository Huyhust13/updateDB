#include "mqttClientUpdate.h"
#include "glog/logging.h"

MqttClientUpdate::MqttClientUpdate(QObject *parent) : QObject(parent)
{
#ifdef LOG_DEBUG_NO
    LOG(INFO) << "[DEBUG] " << __func__;
#endif

    m_client = new QMqttClient(this);

    m_deviceName = QStringLiteral("%1-%2").arg(qAppName()).
            arg(QRandomGenerator::system()->generate()%100000);

    LOG(INFO) << "<" << __func__ << "> Device Name: " << m_deviceName.toStdString();

    connect(m_client, &QMqttClient::messageReceived, this, &MqttClientUpdate::messageReceived);
    connect(m_client, &QMqttClient::connected, this, &MqttClientUpdate::subscribe);
    connect(m_client, &QMqttClient::stateChanged, this, &MqttClientUpdate::stateChanged);

    m_client->setHostname("localhost");
    m_client->setPort(1883);

    m_client->connectToHost();
    m_topics = MQTT_TOPIC_COMMON;

    QSettings setting("updateDB.conf", QSettings::IniFormat);
    QString dbPath = setting.value("dbPath", "UNKNOWN").toString();
    QString dbTable = setting.value("dbTable", "UNKNOWN").toString();

    sqlitedb.setDBPath(dbPath);
    sqlitedb.setDBTable(dbTable);
    sqlitedb.initDatabase();
}

void MqttClientUpdate::subscribe()
{
    m_subscription = m_client->subscribe(m_topics, 2);
    if(!m_subscription){
        LOG(ERROR) << "Could not subscribe || topic: " << m_topics.toStdString();
    }
    else{
        LOG(INFO) << "Subscribed " << m_topics.toStdString() << " successful!";
    }
}

void MqttClientUpdate::stateChanged(QMqttClient::ClientState state)
{
    switch (state) {
    case QMqttClient::Connecting:
        LOG(INFO) << "[mqttClientUpdate] Connecting...";
        break;
    case QMqttClient::Connected:
        LOG(INFO) << "[mqttClientUpdate] Connected!";
        //        subscribe();
        break;
    case QMqttClient::Disconnected:
        LOG(INFO) << "[mqttClientUpdate] Disconnected!";
        break;
    }
}

void MqttClientUpdate::messageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    QString content = tr("Received message size '%1' from topic '%2'")
            .arg(message.size()).arg(topic.name());
    LOG(INFO) << content.toStdString() << std::endl;

    if(topic == MQTT_SUBTOPIC_REQUEST_RECEIVED){
        /// received request from Window Form
        /// Export List IDs and publish to Broker
        sqlitedb.openDatabase();
        QJsonDocument jsondoc = sqlitedb.exportIDs();
        sqlitedb.closeDatabase();
        m_client->publish((MQTT_TOPIC_REQUEST_RESPONSE), jsondoc.toJson());
        LOG(INFO) << "[mqttClientUpdate] published ListIDs to "<< MQTT_TOPIC_REQUEST_RESPONSE.toStdString();
    }

    if(topic == MQTT_SUBTOPIC_DBUPDATED_RECEIVED){
        /// Received DB_Data update from window form
        /// Update Local DB follow window form
        sqlitedb.openDatabase();
        updateDB(message);
        sqlitedb.closeDatabase();
    }

}

MqttClientUpdate::~MqttClientUpdate(){};

/**
 * @brief MqttClientUpdate::updateDB - Nhan json message
 *        phan tach ra tung phan: add, remove, update de thuc hien cap nhat db
 * @param message
 * @return - 0 - Neu thanh cong
 *           1 - Neu that bai
 */
int MqttClientUpdate::updateDB(QByteArray message)
{
    message.replace("\'", "\"");
    QString messageStr = QString(message);

#ifdef LOG_DEBUG_NO
    LOG(INFO) << "message update: " << messageStr.toStdString();
#endif

    json j = json::parse(message);
    uint cnt = 0;
    uint total = j["add"].size();
#if 1
    ///< --- ADD NEW RECORD TO DB ---
    for (uint i=0; i < total; i++) {
        PersonInfo person;
        json json_person = j["add"][i];
        try {
            if(json_person["id"] != nullptr){
                person.recognize_id = QString::fromStdString(json_person["id"]);
            }
            else{
                throw;
            }

            if(json_person["fullName"] != nullptr){
                person.fullName = QString::fromStdString(json_person["fullName"]);
            }
            else{
                throw;
            }

            if(json_person["department"] != nullptr){
                person.department = QString::fromStdString(json_person["department"]);
            }
            else{
                person.department = "UNKNOWN";
            }

            if(json_person["company"] != nullptr){
                person.company = QString::fromStdString(json_person["company"]);
            }
            else{
                person.company = "UNKNOWN";
            }

            // Chu su dung phan nay
//            if(json_person["level"] != nullptr){
//                person.level = QString::fromStdString(json_person["level"]);
//            }
//            else{
//                person.level = 1;
//            }

            if(json_person["avt"] != nullptr){
                QByteArray base64Avt = QByteArray::fromStdString(json_person["avt"]);
                person.avtByteArray = base64Avt;
            }

            if(json_person["ver"] != nullptr){
                person.ver = json_person["ver"];
            }

            if(sqlitedb.insertPerson(person)){
                LOG(INFO) << "[INSERT] inserted " << person.fullName.toStdString() << " successful!";
                cnt ++;
            }
            else{ throw;}

        } catch (json::exception &e) {
            LOG(ERROR) << e.what();
            addedFail_jsonVec.push_back(json_person);
            LOG(INFO) << "<" << __func__ << "> Have " << addedFail_jsonVec.size() << " json added Fail";
            return 1;
        }

    }
    LOG(INFO) << "[UPDATE-DB] Added: " << cnt << " new done! " << total-cnt << " fail.";

    ///< ------ REMOVE --------
    cnt = 0;
    try {
        total = j["remove"].size();
        for (uint i=0; i < total; i++){
            QString removed_per = QString::fromStdString(j["remove"][i]);
            if(sqlitedb.removePerson(removed_per)){
                LOG(INFO) << "[DB-REMOVE] removed " << removed_per.toStdString();
                cnt++;
            }
            else{
                removedFail_jsonVec.push_back(removed_per);
            }
        }
        LOG(INFO) << "[UPDATE-DB] Removed: " << cnt << " done! " << total-cnt << " fail.";
    }
    catch (json::exception &e){
        LOG(ERROR) << e.what();
        return 1;
    }

    ///< ADD UPDATE TO VECTOR
    cnt = 0;
    total = j["update"].size();
    for (uint i=0; i < total; i++) {
        PersonInfo person;
        json json_person = j["update"][i];
        try {
            if(json_person["id"] != nullptr){
                person.recognize_id = QString::fromStdString(json_person["id"]);
            }
            else{
                throw;
            }

            if(json_person["fullName"] != nullptr){
                person.fullName = QString::fromStdString(json_person["fullName"]);
            }
            else{
                throw;
            }

            if(json_person["department"] != nullptr){
                person.department = QString::fromStdString(json_person["department"]);
            }
            else{
                person.department = "UNKNOWN";
            }

            if(json_person["company"] != nullptr){
                person.company = QString::fromStdString(json_person["company"]);
            }
            else{
                person.company = "UNKNOWN";
            }

            if(json_person["level"] != nullptr){
                person.level = QString::fromStdString(json_person["level"]);
            }
            else{
                person.level = 1;
            }

            if(json_person["avt"] != nullptr){
                QByteArray base64Avt = QByteArray::fromStdString(json_person["avt"]);
                //                QImage avt_img;
                //                avt_img.loadFromData(QByteArray::fromBase64(base64Avt));
                //                person.avt = avt_img;
                person.avtByteArray = base64Avt;
#if 0
                LOG(INFO) << person.avt.bits();
#endif
            }

            if(json_person["ver"] != nullptr){
                person.ver = json_person["ver"];
            }

            if(sqlitedb.updateInfo(person)){
                LOG(INFO) << "[DB-UPDATE] updated: " << person.fullName.toStdString();
                cnt++;
            }
            else{
                throw ;
            }

        } catch (json::exception &e) {
            LOG(ERROR) << e.what();
            updatedFail_jsonVec.push_back(json_person);
            LOG(INFO) << "[DB-UPDATE] Update fail";
            return 1;
        }

    }
    LOG(INFO) << "[UPDATE-DB] updated: " << cnt << " done! " << total-cnt << " fail.";

    ///< --- REPORT to Window app -->
    QJsonDocument jsondoc = sqlitedb.exportIDs();
    m_client->publish((MQTT_TOPIC_REQUEST_RESPONSE), jsondoc.toJson());

#endif
    return 0;
}
