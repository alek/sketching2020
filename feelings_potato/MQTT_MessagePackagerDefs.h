
#ifndef MQTTDEFS
#define MQTTDEFS
// define a new type that is a function pointer
typedef String (*messsage_function)(void);

struct MQTT_Object {
  long lastTimeSent;
  int interval;
  messsage_function getMessage;
  char tag[];
};
#endif
