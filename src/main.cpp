#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <RBDdimmer.h>
#include <SPI.h>
#include <WEMOS_SHT3X.h>
#include <Wire.h>

#define pin_PWM D3
#define pin_ButtonUp D5
#define pin_ButtonDown D8
#define pin_ZeroCross D7

Adafruit_SSD1306 display(D6);
SHT3X sht30(0x45);
dimmerLamp dimmer(pin_PWM, pin_ZeroCross);

bool up_Pressed = false;
bool down_Pressed = false;
float lastC = 0;
int power = 0;

void setup() {
  pinMode(pin_PWM, OUTPUT);
  pinMode(pin_ZeroCross, INPUT);
  pinMode(pin_ButtonUp, INPUT);
  pinMode(pin_ButtonDown, INPUT);

  dimmer.setPower(0);
  dimmer.begin(NORMAL_MODE, ON);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  if (digitalRead(pin_ButtonUp) == true) {
    up_Pressed = false;
    if (power < 100) power += 1;
    dimmer.setPower(power);
  }
  if (digitalRead(pin_ButtonDown) == true) {
    down_Pressed = false;
    Serial.println("DOWN");
    if (power > 0) power -= 1;
    dimmer.setPower(power);
  }

  if (sht30.get() == 0) {
    if (lastC == 0) {
      lastC = sht30.cTemp;
    } else {
      lastC = (lastC * 99 / sht30.cTemp) / 100;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(sht30.cTemp);
  display.println("C");
  display.print(int(round(power * 2.5)));
  display.println("W");
  display.print(power);
  display.println("%");

  display.display();
}