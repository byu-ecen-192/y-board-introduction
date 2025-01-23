#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "Arduino.h"
#include "SparkFun_LIS2DH12.h"
#include "yboard.h"

// Screen Constants
#define BRIGHTNESS_DAMPER 0.8 // 0 is brightest
#define REFRESH_RATE 50       // Measured in ms
#define ZERO_DEG 0

#define ON 1

Adafruit_SSD1306 display(128, 64); // Create display

void setup() {
  Serial.begin(9600);
  Yboard.setup();

  // test speaker
  Yboard.play_notes("O5 F D. C#8 D. C#8 D8 B8- O4 G8 F"); // Play fight song

  // Display setup
  delay(1000); // Display needs time to initialize
  display.begin(SSD1306_SWITCHCAPVCC,
                0x3c); // Initialize display with I2C address: 0x3C
  display.clearDisplay();
  display.setTextColor(1);
  display.setRotation(ZERO_DEG); // Can be 0, 90, 180, or 270
  display.setTextWrap(false);
  display.display();
}

void loop() {
  display.clearDisplay();

  // Test switches
  display.drawRoundRect(0, 40, 12, 24, 3, ON);
  if (Yboard.get_switch(1)) {
    Yboard.set_led_color(1, 255, 255, 255);
    display.fillRoundRect(2, 42, 8, 8, 3, ON);
  } else {
    Yboard.set_led_color(1, 0, 0, 0);
    display.fillRoundRect(2, 54, 8, 8, 3, ON);
  }

  display.drawRoundRect(14, 40, 12, 24, 3, ON);
  if (Yboard.get_switch(2)) {
    Yboard.set_led_color(2, 255, 255, 255);
    display.fillRoundRect(16, 42, 8, 8, 3, ON);
  } else {
    Yboard.set_led_color(2, 0, 0, 0);
    display.fillRoundRect(16, 54, 8, 8, 3, ON);
  }

  // Test buttons
  display.drawRect(30, 45, 17, 17, ON);
  if (Yboard.get_button(1)) {
    Yboard.set_led_color(3, 255, 255, 255);
    display.fillCircle(38, 53, 6, ON);
  } else {
    Yboard.set_led_color(3, 0, 0, 0);
    display.drawCircle(38, 53, 6, ON);
  }

  display.drawRect(50, 45, 17, 17, ON);
  if (Yboard.get_button(2)) {
    Yboard.set_led_color(4, 255, 255, 255);
    display.fillCircle(58, 53, 6, ON);
  } else {
    Yboard.set_led_color(4, 0, 0, 0);
    display.drawCircle(58, 53, 6, ON);
  }

  // Test Knob
  int knob_value = map(Yboard.get_knob(), 0, 100, 0, 255);
  Yboard.set_led_color(5, knob_value, knob_value, knob_value);
  display.drawCircle(110, 54, 8, ON);
  int knob_x = cos(knob_value * PI / 180 + PI / 4) * 8 + 110;
  int knob_y = sin(knob_value * PI / 180 + PI / 4) * 8 + 54;
  display.fillCircle(knob_x, knob_y, 2, ON);

  // Test accelerometer
  if (Yboard.accelerometer_available()) {
    accelerometer_data accel_data = Yboard.get_accelerometer();
    int x = accel_data.x;
    int y = accel_data.y;
    int z = accel_data.z;

    if (x > 1000) {
      x = 1000;
    } else if (x < -1000) {
      x = -1000;
    }

    if (y > 1000) {
      y = 1000;
    } else if (y < -1000) {
      y = -1000;
    }

    if (z > 1000) {
      z = 1000;
    } else if (z < -1000) {
      z = -1000;
    }

    if (x < 0) {
      Yboard.set_led_color(6, map(x, 0, -1000, 0, 255), 0, 0);
    } else {
      Yboard.set_led_color(6, 0, map(x, 0, 1000, 0, 255),
                           map(x, 0, 1000, 0, 255));
    }

    if (y < 0) {
      Yboard.set_led_color(7, map(y, 0, -1000, 0, 255), 0,
                           map(y, 0, -1000, 0, 255));
    } else {
      Yboard.set_led_color(7, 0, map(y, 0, 1000, 0, 255), 0);
    }

    if (z < 0) {
      Yboard.set_led_color(8, map(z, 0, -1000, 0, 255),
                           map(z, 0, -1000, 0, 255), 0);
    } else {
      Yboard.set_led_color(8, 0, 0, map(z, 0, 1000, 0, 255));
    }

    uint8_t text_size = 2;
    display.setTextSize(text_size);
    display.setCursor(0, 0);
    display.printf("x = %i", (int)accel_data.x);
    display.setCursor(0, 12);
    display.printf("y = %i", (int)accel_data.y);
    display.setCursor(0, 24);
    display.printf("z = %i", (int)accel_data.z);
  }

  display.display(); // Draw on display
}
