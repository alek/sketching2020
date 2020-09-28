#include "MQTT_MessagePackagerDefs.h"
//-------------------------------------   POEM

const int lineLengthOfPoem = 4;
String lines[lineLengthOfPoem] = { "The lamp once out", "Cool stars enter", "The window frame.", "- Natsume Soseki" };
int lineInterval[lineLengthOfPoem] = { 10000, 5000, 7000, 50000 };
int whichLine = 0;

void updatePoemLine() {
      whichLine = whichLine + 1;
      if (whichLine > (lineLengthOfPoem -1) ) {
      whichLine = 0;
    }   
}

int updatePoemLineInterval(MQTT_Object* mqtto) {
  mqtto->interval = lineInterval[whichLine];
}

String poemMessage() {
    return lines[whichLine];
}

MQTT_Object poemMessageObject = {
  .lastTimeSent = 0,
  .interval = lineInterval[0],
  .getMessage = poemMessage,
   {.tag = "try/table_7/poem"}
};
