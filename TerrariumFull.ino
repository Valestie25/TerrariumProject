#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#define dht_dpin D4
#include <BlynkSimpleEsp8266.h>
#include <PubSubClient.h>

const char* ssid = "Totalplay-419C";
const char* password = "419C8DFD9jVhmqg3";
const char* mqtt_broker = "broker.hivemq.com";

int buzzer = D2;
int smokeA0 = A0;
// Your threshold value. You might need to change it.
int sensorThres = 500;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lasMsg = 0;
char OutTopic[] = "/UTT/0319125260/";
char InTopic[] = "/UTT/0319125260/cntrl/";
int Ts = 12000; //milliseconds sampling rate
int port = 1883;
DHT dht(dht_dpin, DHTTYPE); 

void setup_wifi() {
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // put your main code here, to run repeatedly:
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
if((char)payload[0] == '2'){
    digitalWrite(BUILTIN_LED, LOW);
  }else{
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  while (!client.connected()){
    Serial.print("Attempting MQTT connection...");
    String clientId = "NMCI-0319125260";
    if(client.connect(clientId.c_str())){
      Serial.println("connected");
      client.subscribe(InTopic);
      }else{
        Serial.print("failed, rc= ");
        Serial.print(client.state());
        Serial.println("try again in 5 seconds");
        delay(2000);
      }
    }
  }

void setup()
{ 
  
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_broker, port);
  client.setCallback(callback);

  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature\n\n");
  delay(7000);

}

void loop() {
   if(!client.connected()){
    reconnect();
    }
    client.loop();
    unsigned long now = millis();
    if(now - lasMsg > Ts){
      float analogSensor = analogRead(smokeA0);
      float ToC = dht.readHumidity();
      float RH = dht.readTemperature();   
      String JSON_msg = "{\"n\":\"0319125260\",\"ToC\":" + String(ToC, 2);
             JSON_msg += ",\"RH\":" + String(RH, 0);
             JSON_msg += ",\"Air0\":" + String(analogSensor, 0) + "}\n";
      Serial.print("Publish message");
      Serial.println(JSON_msg);
      char JSON_msg_array[60];
      int JSON_msg_length = JSON_msg.length();
      JSON_msg.toCharArray(JSON_msg_array, 60);
      Serial.println(JSON_msg_array);
      if (client.connected()){
        client.publish(OutTopic, JSON_msg_array);
        Serial.print("Published to topic: ");
        Serial.println(OutTopic);
        }else{
          Serial.print("Not connected to broker... couldn't send MQTT message...");
        }
        if (analogSensor > sensorThres)
       {
         tone(buzzer, 1000, 9000);
         Serial.print("Alert: Fire in the House\n");  
       }
       else
       {
         noTone(buzzer);
       }
        lasMsg = millis();
      }
}
