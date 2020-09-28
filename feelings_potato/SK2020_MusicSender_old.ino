// /*
//   Based on MQTT Client sender
//   created 11 June 2020
//   by Tom Igoe
//
//   This sketch uses https://shiftr.io/try as the MQTT broker.
//
//   the arduino_secrets.h file:
//   #define SECRET_SSID ""    // network name
//   #define SECRET_PASS ""    // network password
//   #define SECRET_MQTT_USER "" // broker username
//   #define SECRET_MQTT_PASS "" // broker password
//
//
//   Edited 2020 09 25
//   Sketching in Hardware
//   by Carlyn Maw
//
//   New code sends various types of messages.
// */
//
// #include <WiFiNINA.h>
// #include <ArduinoMqttClient.h>
// #include "arduino_secrets.h"
//
// // initialize WiFi connection:
// WiFiClient wifi;
// MqttClient mqttClient(wifi);
//
// // details for MQTT client:
// char broker[] = "broker.shiftr.io";
// int port = 1883;
// //char topicRoot[] = "try/table_7/";
// char clientID[] = "voice01";
//
// // define a new type that is a function pointer
// typedef String (*messsage_function)(void);
//
// struct MQTT_Object {
//   long lastTimeSent;
//   int interval;
//   messsage_function getMessage;
//   char tag[];
// };
//
// void sendMQTTObject(MQTT_Object* mqtto) {
//     if (millis() - mqtto->lastTimeSent > mqtto->interval) {
//     // start a new message on the topic:
//     mqttClient.beginMessage(mqtto->tag);
//     // add a random number as a numeric string (print(), not write()):
//     mqttClient.print(mqtto->getMessage());
//     mqttClient.endMessage();
//     mqtto->lastTimeSent = millis();
//   }
// }
//
// //-------------------------------------   RANDOM MESSAGE
// String randomMessage() {
//   return String(random(127));
// }
//
// MQTT_Object randomMessageObject = {
//   .lastTimeSent = 0,
//   .interval = 5000,
//   .getMessage = randomMessage,
//   {.tag = "try/table_7/random"}
// };
//
// //-------------------------------------   STEPPING TONE
// int incrementTone;
// int nextToneStep;
// int incrementToneMax = 127;
// int incrementToneMin = 0;
//
// String scaleToneMessage() {
//
//     //Serial.print(incrementTone);
//     incrementTone = incrementTone + nextToneStep;
//
//     if (incrementTone >= incrementToneMax) {
//       nextToneStep = -1;
//     } else if (incrementTone <= incrementToneMin) {
//       nextToneStep = 1;
//     }
//     return String(incrementTone);
//
// }
//
// MQTT_Object scaleToneMessageObject = {
//   .lastTimeSent = 0,
//   .interval = 3000,
//   .getMessage = scaleToneMessage,
//    {.tag = "try/table_7/scale"}
// };
//
// //-------------------------------------   POEM
//
// const int lineLengthOfPoem = 4;
// String lines[lineLengthOfPoem] = { "The lamp once out", "Cool stars enter", "The window frame.", "- Natsume Soseki" };
// int lineInterval[lineLengthOfPoem] = { 10000, 5000, 7000, 50000 };
// int whichLine = 0;
//
// void updatePoemLine() {
//       whichLine = whichLine + 1;
//       if (whichLine > (lineLengthOfPoem -1) ) {
//       whichLine = 0;
//     }
// }
//
// int updatePoemLineInterval(MQTT_Object* mqtto) {
//   mqtto->interval = lineInterval[whichLine];
// }
//
// String poemMessage() {
//     return lines[whichLine];
// }
//
// MQTT_Object poemMessageObject = {
//   .lastTimeSent = 0,
//   .interval = lineInterval[0],
//   .getMessage = poemMessage,
//    {.tag = "try/table_7/poem"}
// };
//
// //-------------------------------------   POTENTIOMETER, SIMPLE
// const int voiceSensor = 0;
//
// String getValueForMessage() {
//   int val = analogRead(voiceSensor);
//   return String((uint8_t) (map(val, 0, 1023, 0, 127)));
// }
//
// MQTT_Object potMessageObject = {
//   .lastTimeSent = 0,
//   .interval = 3000,
//   .getMessage = getValueForMessage,
//    {.tag = "try/table_7/carlyn/dial"}
// };
//
//
// //----------------------------------------------------   SETUP
// void setup() {
//   // initialize serial:
//   Serial.begin(9600);
//   // wait for serial monitor to open:
//   while (!Serial);
//
//   // initialize WiFi, if not connected:
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.print("Connecting to ");
//     Serial.println(SECRET_SSID);
//     WiFi.begin(SECRET_SSID, SECRET_PASS);
//     delay(2000);
//   }
//   // print IP address once connected:
//   Serial.print("Connected. My IP address: ");
//   Serial.println(WiFi.localIP());
//
//   // set the credentials for the MQTT client:
//   mqttClient.setId(clientID);
//   // login to the broker with a username and password:
//   mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);
//
//   // try to connect to the MQTT broker once you're connected to WiFi:
//   while (!connectToBroker()) {
//     Serial.println("attempting to connect to broker");
//     delay(1000);
//   }
//   Serial.println("connected to broker");
// }
//
//
// //----------------------------------------------------   LOOP
// void loop() {
//   // if not connected to the broker, try to connect:
//   if (!mqttClient.connected()) {
//     Serial.println("reconnecting");
//     connectToBroker();
//   }
//
//   sendMQTTObject(&randomMessageObject);
//   sendMQTTObject(&scaleToneMessageObject);
//
//   updatePoemLine();
//   updatePoemLineInterval(&poemMessageObject);
//   sendMQTTObject(&poemMessageObject);
//
//   sendMQTTObject(&potMessageObject);
//
// }
//
// //----------------------------------------------------   END LOOP
//
//
// //-------------------------------------   connectToBroker()
//
// boolean connectToBroker() {
//   // if the MQTT client is not connected:
//   if (!mqttClient.connect(broker, port)) {
//     // print out the error message:
//     Serial.print("MOTT connection failed. Error no: ");
//     Serial.println(mqttClient.connectError());
//     // return that you're not connected:
//     return false;
//   }
//   // once you're connected, you
//   // return that you're connected:
//   return true;
// }
