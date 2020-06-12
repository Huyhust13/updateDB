#include "mqttClientUpdate.h"
#include "glog/logging.h"

MqttClientUpdate::MqttClientUpdate(QObject *parent) : QObject(parent)
{
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
    m_topics = "topic/#";

//    SQLiteDB sqlitedb;
    sqlitedb.setDBName("db/cts_client.db");
    sqlitedb.setDBTable("CTS");
}

void MqttClientUpdate::subscribe()
{
    m_subscription = m_client->subscribe(m_topics, 2);
    if(!m_subscription){
        LOG(ERROR) << "Could not subscribe || topic: " << m_topics.toStdString();
    }
    else{
        LOG(INFO) << "Subscribed " << m_topics.toStdString() << "successful!";
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
        subscribe();
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

    if(topic == tr("topic/GeneralExternalRequests")){
        /// received request from Window Form
        /// Export List IDs and publish to Broker
        QJsonDocument jsondoc = sqlitedb.exportIDs();
        m_client->publish(tr("topic/DB_Status_Ack"), jsondoc.toJson());
        LOG(INFO) << "[mqttClientUpdate] published ListIDs to 'topic/DB_Status_Ack'";
    }

    if(topic == tr("topic/DB_Data")){
        /// Received DB_Data update from window form
        /// Update Local DB follow window form
        updateDB(message);


    }

}

//void MqttClientUpdate::receivedDBUpdatedData()
//{
//    LOG(INFO) << "<" << __func__ << ">";
////    return 1;
//}

MqttClientUpdate::~MqttClientUpdate(){};

int MqttClientUpdate::updateDB(QByteArray message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message);
    QJsonObject jObject = doc.object();
//    QJsonArray


#if 0
        // push info need to add to added_vec<PersonInfo>
        LOG(INFO) << "Push to added_vec<PersonInfo>";
        for (uint i=0; i < j["add"].size(); i++) {
            PersonInfo person;
            json json_person = j["add"][i];
            try {
                if(json_person["id"] != nullptr){
                    person.recognize_name = QString::fromStdString(json_person["id"]);
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

                added_vec.push_back(person);
                LOG(INFO) << "<" << __func__ << "> " << i << " - recognize_name" << j["add"][i]["id"];

            } catch (json::exception &e) {
                LOG(ERROR) << e.what();
                addedFail_jsonVec.push_back(json_person);
                LOG(INFO) << "<" << __func__ << "> Have " << addedFail_jsonVec.size() << " json added Fail";
            }

        }

        LOG(INFO)<< "push recognize_name need to remove to removed_vec";
        for (uint i=0; i < j["remove"].size(); i++){
            removed_vec.push_back(QString::fromStdString(j["remove"][i]));
            LOG(INFO) << "<" << __func__ << "> Have " << removed_vec.size() << " have to remove";
    //        LOG(INFO) << "add remove: " << j["remove"][i];
        }


        ///< ADD UPDATE TO VECTOR
        LOG(INFO) << "Push to added_vec<PersonInfo>";
        for (uint i=0; i < j["update"].size(); i++) {
            PersonInfo person;
            json json_person = j["update"][i];
            try {
                if(json_person["id"] != nullptr){
                    person.recognize_name = QString::fromStdString(json_person["id"]);
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

                updated_vec.push_back(person);

            } catch (json::exception &e) {
                LOG(ERROR) << e.what();
                updatedFail_vec.push_back(json_person);
                LOG(INFO) << "<" << __func__ << "> Had " << addedFail_jsonVec.size() << " info add to updated Fail";
            }

        }

#endif
        return 0;

}
