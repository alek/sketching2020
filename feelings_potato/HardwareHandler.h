#include "MQTT_MessagePackagerDefs.h"
//20*5*number
//20Hz - 655Hz

// ------------------------------------------------------
// ---------------------------------------------- INPUTS
// ------------------------------------------------------

//---------------------------------  NOSE
const int pushButton = 4;
const bool trueState = LOW;
bool buttonState = false;

//---------------------------------  HUNGER
const int hungerPin = 0;  //A0 input

//String getValueForMessage(int sensorPin) {
//  int val = analogRead(sensorPin);
//  //return "dial it in";
//  return String((uint8_t) (map(val, 0, 1023, 0, 127)));
//}

String getHungerValueForMessage() {
  int val = analogRead(hungerPin);
  return String((uint8_t) (map(val, 0, 1023, 0, 127)));
}

//---------------------------------  HUNGER OBJECT
MQTT_Object hungerMessageObject = {
  .lastTimeSent = 0,
  .interval = 3000,
  .getMessage = getHungerValueForMessage,
   {.tag = "try/table_7/feelingsPotato/hunger"}
};


//---------------------------------  FOLICLES
const int folicleCount = 4;
const int folicle1 = 7;
const int folicle2 = 6;
const int folicle3 = 5;
const int folicle4 = 4;

int folicles[folicleCount] = { folicle1, folicle2, folicle3, folicle4 };
int folicleReadings[folicleCount] = { 0, 0, 0, 0 };



void checkButton() {
  if (trueState == digitalRead(pushButton)) {
    buttonState = true;
  } else {
    buttonState = false;
  }
}


int readFolicle(int folicle) {
  //  if (Serial) {
  //    Serial.print("readFolicle ");
  //    Serial.print(folicle);
  //    Serial.print(" result: ");
  //  }

  int val = analogRead(folicle);
  uint8_t reading = (uint8_t) (map(val, 0, 1023, 0, 255));
  //  if (Serial) {
  //    Serial.println(reading);
  //  }
  return reading;
}

void updateFolicleData() {
  for (int i = 0; i < folicleCount; i++) {
    folicleReadings[i] = readFolicle(folicles[i]);
  }
}

void printFolicleData() {
  for (int i = 0; i < folicleCount; i++) {
    if (Serial) {
      Serial.print("readFolicle ");
      Serial.print(i);
      Serial.print(" result: ");
      Serial.println(folicleReadings[i]);
    }
  }
}

float average (int nums[], int sizeOfArray) {
  long sum = 0;
  for (int i = 0 ; i < sizeOfArray ; i++) {
    sum += nums[i];
  }
  return  ((float) sum) / sizeOfArray ;  // average will be fractional, so float may be appropriate.
}

float averageFolicleData() {
  return average(folicleReadings, folicleCount);
}

void printFolicleAverage() {
     if (Serial) {
      Serial.print("Folicle Average: ");
      Serial.println(averageFolicleData());
    }
}

String getStrokeValueForMessage() {
   uint8_t message = (uint8_t) (map(averageFolicleData(), 0, 255, 0, 127));
  return String(message);
}

//---------------------------------  FOLICLE OBJECT
MQTT_Object strokeMessageObject = {
  .lastTimeSent = 0,
  .interval = 5000,
  .getMessage = getStrokeValueForMessage,
   {.tag = "try/table_7/feelingsPotato/stroke"}
};

// ------------------------------------------------------
// ---------------------------------------------- OUTPUTS
// ------------------------------------------------------

//----------------------------------------------  CILIA
const int cilliaCount = 4;
const int cillia1 = 12;
const int cillia2 = 11;
const int cillia3 = 10;
const int cillia4 = 9;

int cillia[cilliaCount] = { cillia1, cillia2, cillia3, cillia4 };
int cilliaLevels[cilliaCount] = { 0, 0, 0, 0 };

void copyArray(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void updateCilliaData() {
  copyArray(folicleReadings, cilliaLevels, cilliaCount);
}

void printCiliaData() {
  for (int i = 0; i < cilliaCount; i++) {
    if (Serial) {
      Serial.print("cillia ");
      Serial.print(i);
      Serial.print(" val: ");
      Serial.println(cilliaLevels[i]);
    }
  }
}
