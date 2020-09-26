/*
  MQTT Client with a button and an LED

  This sketch demonstrates an MQTT client that connects to a broker, subsrcibes to a topic,
  and both listens for messages on that topic and sends messages to it. When a pushbutton
  is pressed, the client sends a message, a random number between 0 and 15.
  When the client receives a message, it parses it, and if the number is greater than 0, 
  it sets an LED to full. When nothing is happening,
  if the LED is not off, it's faded down one point every time through the loop.

  This sketch uses https://shiftr.io/try as the MQTT broker.

  the circuit:
  - pushbutton attached to pin 2, connected to ground
  - LED's anode connected to pin 3, cathode connected to ground.

  the arduino_secrets.h file:
  #define SECRET_SSID ""    // network name
  #define SECRET_PASS ""    // network password
  #define SECRET_MQTT_USER "" // broker username
  #define SECRET_MQTT_PASS "" // broker password

  created 11 June 2020
  by Tom Igoe
*/

#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Arduino_LSM6DS3.h>
#include "arduino_secrets.h"

#define debug_readings 0
#define include_gyro 0


// initialize WiFi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

// details for MQTT client:
char broker[] = "broker.shiftr.io";
int port = 1883;
char topic_all[] = "try/table_7/chris/#";
char topicx[] = "try/table_7/chris/x";
char topicy[] = "try/table_7/chris/y";
char topicz[] = "try/table_7/chris/z";
char topicpot[] = "try/table_7/chris";
char clientID[] = "buttonClient";
bool state = false;

// intensity of LED:
int intensity = 0;

// details for pushbutton and LED:
const int buttonPin = 2;
const int ledPin = 3;
const int potPin = A7;
const int debounceDelay = 5;
int lastButtonState = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // wait for serial monitor to open:
  while (!Serial);

  // initialize I/O pins:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode (ledPin, OUTPUT);
  pinMode(potPin, INPUT);

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
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  // try to connect to the MQTT broker once you're connected to WiFi:
  while (!connectToBroker()) {
    Serial.println("attempting to connect to broker");
    delay(1000);
  }
  Serial.println("connected to broker");
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), blink, LOW);
  
}

void loop() {
  // if not connected to the broker, try to connect:
  if (!mqttClient.connected()) {
    Serial.println("reconnecting");
    connectToBroker();
  }

  // if a message comes in, read it:
  if (mqttClient.parseMessage() > 0) {
    Serial.print("Got a message on topic: ");
    Serial.println(mqttClient.messageTopic());
    // read the message:
    while (mqttClient.available()) {
      // convert numeric string to an int:
      int message = mqttClient.parseInt();
      Serial.println(message);
      // if the message is greater than 0, set the LED to full intensity:
      if (message > 0) {
        intensity = 255;
      }
    }
  }
  // if the LED is on:
  if (intensity > 0) {
    // update its level:
    analogWrite(ledPin, intensity);
    // fade level down one point for next time through loop:
    intensity = max(intensity--, 0);
  }

  

  if (state)
  {  
    
    // and it's pressed:
    if (HIGH) {
        float x, y, z;
        if (IMU.gyroscopeAvailable()) {
          IMU.readGyroscope(x, y, z);
          if (debug_readings == 1)
          {
            Serial.print(x);
            Serial.print('\t');
            Serial.print(y);
            Serial.print('\t');
            Serial.println(z);
          }
        }

      if (include_gyro == 1)
      {
        // start a new message on the topic:
        mqttClient.beginMessage(topicx);
        String x_str = String(x);
        mqttClient.print(x_str);
        // send the message:
        mqttClient.endMessage();
  
        mqttClient.beginMessage(topicy);
        String y_str = String(y);
        mqttClient.print(y_str);
        // send the message:
        mqttClient.endMessage();
  
        mqttClient.beginMessage(topicz);
        String z_str = String(z);
        mqttClient.print(z_str);
        // send the message:
        mqttClient.endMessage();
      }
      
      int reading=0;
      int avg8=0;
      int avg=0;
      for(int i = 0; i<8; i++)
      {
        reading = analogRead(A7);
        if (debug_readings == 1)
        {
          Serial.print("Reading");
          Serial.print(i);
          Serial.print("= ");
          Serial.println(reading);
        }
        avg8 += reading>>3;
        //Serial.println(avg8);
      }
      avg = avg8 >> 3;
      mqttClient.beginMessage(topicpot);
      mqttClient.print(avg);
      if (debug_readings == 1)
        {
          Serial.println(avg);
        }
      // send the message:
      mqttClient.endMessage();

    }
    delay(1000);
 }
 
}

boolean connectToBroker() {
  // if the MQTT client is not connected:
  if (!mqttClient.connect(broker, port)) {
    // print out the error message:
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    // return that you're not connected:
    return false;
  }
  // once you're connected, you can proceed:
  mqttClient.subscribe(topicpot);
  // return that you're connected:
  return true;
}

void blink()
{
  state = !state;
  Serial.print("State = ");
  Serial.println(state);
}
