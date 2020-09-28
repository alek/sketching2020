/*
  Based on MQTT Client sender
  created 11 June 2020
  by Tom Igoe

  This sketch uses https://shiftr.io/try as the MQTT broker.

  the arduino_secrets.h file:
  #define SECRET_SSID ""    // network name
  #define SECRET_PASS ""    // network password
  #define SECRET_MQTT_USER "" // broker username
  #define SECRET_MQTT_PASS "" // broker password


  Edited 2020 09 25
  Sketching in Hardware
  by Carlyn Maw

  New code sends various types of messages.
*/

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "arduino_secrets.h"
#include "MQTT_MessagePackagerDefs.h"
#include "HardwareHandler.h"

// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "broker.shiftr.io";
int port = 1883;
//char topicRoot[] = "try/table_7/";
char clientID[] = "voice01";

//--------------------------------------------- MQTT_MessagePackager

void sendMQTTObject(MQTT_Object* mqtto) {
  if (millis() - mqtto->lastTimeSent > mqtto->interval) {
    // start a new message on the topic:
    mqttClient.beginMessage(mqtto->tag);
    // add a random number as a numeric string (print(), not write()):
    mqttClient.print(mqtto->getMessage());
    mqttClient.endMessage();
    mqtto->lastTimeSent = millis();
  }
}

//----------------------------------------------------   SETUP
void setup() {
  // pin settings
  pinMode(pushButton, INPUT);
  for (int thisPin = cillia4; thisPin <= cillia1; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  // initialize WiFi, if not connected:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }
  // print IP address once connected:
  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());

  // set the credentials for the MQTT client:
  mqttClient.setId(clientID);
  // login to the broker with a username and password:
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");
}


//----------------------------------------------------   LOOP
void loop() {

  //-------------------------------------  Read Binary Hardware
  checkButton();

  //-------------------------------------  Read Analog Hardware
  updateFolicleData();

  //-------------------------------------  Update Local World

  updateCilliaData();

  if (buttonState == true) {
    printFolicleData();
    Serial.println("------");
    printFolicleAverage();
    Serial.println(" ");
    printCiliaData();
    Serial.println(" ");
  }

  updateCillia();


  //-------------------------------------  Update Remote World
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

  //  sendMQTTObject(&randomMessageObject);
  //  sendMQTTObject(&scaleToneMessageObject);
  //
  //  updatePoemLine();
  //  updatePoemLineInterval(&poemMessageObject);
  //  sendMQTTObject(&poemMessageObject);

  sendMQTTObject(&hungerMessageObject);
  sendMQTTObject(&strokeMessageObject);

}

//----------------------------------------------------   END LOOP


//-------------------------------------   connectToBroker()

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MOTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you
  // return that you're connected:
  return true;
}
