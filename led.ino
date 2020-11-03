#include <WS2812.h>

#define outputPin 13
#define LEDCount 144
#define LedBright 40
#define touchPin 4
#define soundPin 8
#define indicatorLed 143

WS2812 LED(LEDCount);
cRGB currentColor;
cRGB defaultColor;
cRGB indicatorColor;

bool prevIsOn = false;
bool isOn = false;

bool prevTouchState = false;

int delayTime = 100;

int blinkNum = 0;

double rad = 0;

int btSignal;

String btMessage = "";
char btChar;

String commandType = "";
String subjectType = "";
String featureType = "";
String value = "";

void setup() {
  Serial.begin(9600);
  pinMode(touchPin, INPUT);

  LED.setOutput(outputPin);
  LED.setColorOrderGRB();

  defaultColor.r = 0;
  defaultColor.g = 0;
  defaultColor.b = 0;

  indicatorColor.r = 1;
  indicatorColor.g = 1;
  indicatorColor.b = 100;

  currentColor.r = 20;
  currentColor.g = 20;
  currentColor.b = 20;

  for (int i = 0; i < 144; i++) {
    LED.set_crgb_at(i, defaultColor);
  }
  LED.set_crgb_at(indicatorLed, indicatorColor);

  LED.sync();
}

void loop() {
  if (Serial.available()) {
    btMessage = "";
    while (Serial.available()) {
      btChar = Serial.read();
      btMessage.concat(btChar);
    }

    if (sizeof(btMessage) > 0)
      ParseCommandString(btMessage);
  }

  delay(1000);
}

void ParseCommandString(String command) {
  char* cBuf = (char*) malloc(sizeof(char) * command.length() + 1);
  command.toCharArray(cBuf, command.length() + 1);
  Serial.println(cBuf);
  free(cBuf);

  commandType = "";
  int i = 0;
  while (command[i] != '_') {
    commandType.concat(command[i]);
    i++;
  }
  i++;
  Serial.write("commandType: ");

  char* comBuf = (char*) malloc(sizeof(char) * commandType.length() + 1);
  commandType.toCharArray(comBuf, commandType.length() + 1);
  Serial.println(comBuf);
  free(comBuf);

  subjectType = "";
  while (command[i] != '_') {
    subjectType.concat(command[i]);
    i++;
  }
  i++;
  Serial.write("subject");

  featureType = "";
  while (command[i] != '_') {
    featureType.concat(command[i]);
    i++;
  }
  i++;
  Serial.write("feature");

  value = "";
  while (i < command.length()) {
    value.concat(command[i]);
    i++;
  }

  char* valBuf = (char*) malloc(sizeof(char) * value.length() + 1);
  value.toCharArray(valBuf, value.length() + 1);
  Serial.print("value: ");
  Serial.println(valBuf);
  free(valBuf);

  if (commandType == "SET")
  {
    if (subjectType == "LED") {
      if (featureType == "POWER") {
        if (value == "ON")
        {
          Serial.write("switching on");
          switchLed(true);
        } else if (value == "OFF") {
          Serial.write("switching off");
          switchLed(false);
        }
      } else if (featureType == "COLOR") {
        int j = 0;

        String rStr = "";
        while (value[j] != '-') {
          rStr += value[j];
          j++;
        }
        j++;
        int r = rStr.toInt();

        String gStr = "";
        while (value[j] != '-') {
          gStr += value[j];
          j++;
        }
        j++;
        int g = gStr.toInt();

        String bStr = "";
        while (j < value.length()) {
          bStr += value[j];
          j++;
        }
        int b = bStr.toInt();

        changeLedColor(r, g, b);
      }
    }
  } else if (commandType == "GET")
  {

  } else if (commandType == "INF") {

  }
}

void resetLed() {
  for (int i = 0; i < 144; i++) {
    LED.set_crgb_at(i, defaultColor);
  }

  LED.sync();
}

void switchLed(bool state) {
  resetLed();

  cRGB newColor;
  if (state) {
    newColor.r = currentColor.r;
    newColor.g = currentColor.g;
    newColor.b = currentColor.b;
  } else {
    newColor.r = 0;
    newColor.g = 0;
    newColor.b = 0;
  }

  for (int i = 0; i < 50; i++) {
    LED.set_crgb_at(i, newColor);
  }

  LED.set_crgb_at(indicatorLed, indicatorColor);

  LED.sync();
}

void changeLedColor(int r, int g, int b) {
  resetLed();

  currentColor.r = r;
  currentColor.g = g;
  currentColor.b = b;

  cRGB newColor;
  newColor.r = r;
  newColor.g = g;
  newColor.b = b;

  for (int i = 0; i < 50; i++) {
    LED.set_crgb_at(i, newColor);
  }

  LED.set_crgb_at(indicatorLed, indicatorColor);

  LED.sync();
}
