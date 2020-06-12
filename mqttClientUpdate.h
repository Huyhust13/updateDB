#ifndef MQTTCLIENTUPDATE_H
#define MQTTCLIENTUPDATE_H

#include <QObject>
#include <QMqttClient>
#include "QDebug"
#include <QCoreApplication>
#include <QRandomGenerator>
#include <unistd.h>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <sqlitedb.h>

class MqttClientUpdate : public QObject
{
    Q_OBJECT
public:
    explicit MqttClientUpdate(QObject *parent=nullptr);
    virtual ~MqttClientUpdate();

    void subscribe();

private slots:
    void messageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void stateChanged(QMqttClient::ClientState state);
    void subscriptionStateChanged(QMqttSubscription::SubscriptionState state);
    void publish();

//signals:
    void receivedRequest();
    void receivedDBUpdatedData();
    int updateDB(QByteArray message);

private:
    QMqttClient *m_client;
    QString m_hostname;
    quint16 m_port;
    QString m_topics;
    QMqttSubscription *m_subscription;
    QString m_deviceName;

    SQLiteDB sqlitedb;

};

#endif // MQTTCLIENTUPDATE_H
