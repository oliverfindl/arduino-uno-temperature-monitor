/**
 * arduino-uno-temperature-monitor v1.0.0 (2018-03-17)
 * Copyright 2018 Oliver Findl
 * @license MIT
 */

#include <OneWire.h> // https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library
#include <TM1637Display.h> // https://github.com/avishorp/TM1637

#define LIMIT 30
#define INTERVAL 500
#define LED_PIN 13
#define ONE_WIRE_BUS 4
#define CLK 2
#define DIO 3

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
TM1637Display display(CLK, DIO);

float temperature;
char array[4];
byte rawData[4];
const uint8_t AHOJ[] = {
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  // A
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // H
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
  SEG_B | SEG_C | SEG_D | SEG_E                   // J
};

void setup(void) {
  Serial.begin(9600);
  sensors.begin();
  pinMode(LED_PIN, OUTPUT);
  display.setBrightness(1);
  display.setSegments(AHOJ);
}

void loop(void) {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.println(temperature);
  
  if(temperature > LIMIT) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  dtostrf(temperature, 4, 2, array);
  rawData[0] = display.encodeDigit(array[0]);
  rawData[1] = display.encodeDigit(array[1]) | 0x80;
  rawData[2] = display.encodeDigit(array[3]);
  rawData[3] = display.encodeDigit(array[4]);
  display.setSegments(rawData);
  
  delay(INTERVAL);
}
