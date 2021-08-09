#!/usr/bin/python3.9
#sudo chmod a+rw /dev/ttyACM0

import paho.mqtt.client as mqttClient
import json
import pymysql
import datetime
import time

topic1 = "/UTT/0319125260/"

import paho.mqtt.client as mqtt

def on_connect(client, userdata, flags, rc): 
    print("Connected with result code "+str(rc))
    client.subscribe(topic1)

def on_message(client, userdata, msg): 
    json_data = str(msg.payload.decode("utf-8"))
    intopic = str(msg.payload)

    print("Topic: %s, Message: %s" % (intopic, json_data))

    if json_data != "":
        print("Received: %s " % json_data)
        parsed_data = json.loads(json_data)

        node = parsed_data['n']
        temperature = parsed_data['ToC']
        RH = parsed_data['RH']
        air = parsed_data['Air0']

        print("Node: %s " % node)
        print("Temperature: %s " % temperature)
        print("Relative Humidity: %s " % RH)
        print("Air quality: %s " % air)  

        time_sec = time.time()
        # Getting the time in seconds from the imported time module

        st = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # Getting the date in the next format '%Y-%m-%d %H:%M:%S'

        str_ts = str(time_sec).split('.')[0]
        # Getting the current seconds since the epoch, in UTC

        print(str_ts)
        print(st)

        try:
            username = "root"
            password = ""
            database = "terrarium"
            localhost = "127.0.0.1"

            db = pymysql.connect(host=localhost,
                user=username,
                password=password,
                database=database,
                cursorclass=pymysql.cursors.DictCursor)
            
            print("Connected to MySQl... ")
        except pymysql.Error as e:
            print("MySQL Error...", e)
            

        cursor = db.cursor()

        SQL_table = 'values_r'

        SQL_string = "INSERT INTO %s (Temperature, Humidity, Air_Quality) VALUES (%s, %s, %s)" % (SQL_table,temperature, RH, air)                            

        print(SQL_string)

        try:
            cursor.execute(SQL_string)
            db.commit()
            print("Node %s: DB storage success!!" % node)
        except pymysql.Error as e:
            print("MySQL Error...", e)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

try:

    client.connect("broker.hivemq.com", 1883, 60)

    

except Exception as e:
    print("MQTT Error: ", e)

client.loop_forever()
