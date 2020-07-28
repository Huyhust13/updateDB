#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import sys
# This is the Subscriber

# SUBSCRIBE_TOPIC = "toApp/#"

SUBSCRIBE_TOPIC = "toClient/#"

def on_connect(client, userdata, flags, rc):
  print("Connected with result code "+str(rc))
  client.subscribe(SUBSCRIBE_TOPIC)

def on_message(client, userdata, msg):
    data = msg.payload.decode()
    print("Recieved: {}".format(data))
    print("Size of data: " + str(sys.getsizeof(data)))
    print("Type of data: " + str(type(data)))
    import json
    try:
        data = data.replace("\'", "\"")
        data_json = json.loads(data)
        print("Size of data_json: {} byte".format(sys.getsizeof(data_json)))
        print("Type of data_json: " + str(type(data_json)))
        print(data_json)
        # print(data_json['recognize_name'])
    except Exception as e:
        print("[ERROR]" + str(e))

client = mqtt.Client()
client.connect("92.168.1.84",1883,60)

client.on_connect = on_connect
client.on_message = on_message

client.loop_forever()
