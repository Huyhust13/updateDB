#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import json
import sys, time

def publish_request():
    topic_request = "toClient/GeneralExternalRequests"
    data_request = "request"
    m_client.publish(topic_request, data_request)

def pub_db_update():
    updateFile = "update_remove.json"
    # updateFile = "update_add.json"
    # updateFile = "update_update.json"

    with open(updateFile) as jsonFile:
        data = json.load(jsonFile)

    print("Type of data: {}".format(type(data)))
    print("Size of data: {} byte".format(sys.getsizeof(data)))

    topic_db_update = "toClient/DB_Data"
    m_client.publish(topic_db_update, str(data))

if __name__ == "__main__":
    m_client = mqtt.Client()
    broker = "localhost"
    port = 1883
    m_client.connect(broker, port, keepalive=60)

    # Test request

    # publish_request()
    pub_db_update()

