#include <Arduino.h>

#include "yboard.h"

// Screen Constants
#define BRIGHTNESS_DAMPER 0.8 // 0 is brightest
#define REFRESH_RATE 50       // Measured in ms
#define SCREEN_WIDTH 128
#define ZERO_DEG 0
#define PADDING 2

#define SWITCH_X 0 + PADDING * 2
#define SWITCH_Y 11
#define SWITCH_WIDTH 9
#define SWITCH_HEIGHT 20
#define SWITCH_RADIUS 3

#define BUTTON_X 0 + PADDING * 2
#define BUTTON_Y SWITCH_Y + SWITCH_HEIGHT + PADDING * 2
#define BUTTON_WIDTH 17
#define BUTTON_HEIGHT 17

#define KNOB_RADIUS 8
#define KNOB_X SCREEN_WIDTH - KNOB_RADIUS-PADDING*2
#define KNOB_Y 20

#define ON 1
#define OFF 0

int x,y,z,knob,temp,knob_x,knob_y;
bool sw[4];
bool but[5];
bool knob_button_pressed = false;

void setup() {
  Serial.begin(9600);
  Yboard.setup();
  x = 0;
  y = 0;
  z = 0;
  for (int i = 0; i < 5; ++i) {
    but[i] = Yboard.get_button(i + 1);
  }
  for (int i = 0; i < 4; ++i) {
    sw[i] = Yboard.get_switch(i + 1);
  }
  knob_button_pressed = Yboard.get_knob_button();
  temp = 0;

  // Display setup
  delay(1000); // Display needs time to initialize
  Yboard.display.clearDisplay();
  Yboard.display.setTextColor(ON,OFF);
  Yboard.display.setRotation(90); // Can be 0, 90, 180, or 270
  Yboard.display.setTextWrap(false);
  uint8_t text_size = 1;
  Yboard.display.setTextSize(text_size);
  Yboard.display.setCursor(0, 0);
  Yboard.display.printf("x:%i",x);
  Yboard.display.setCursor(42, 0);
  Yboard.display.printf("y:%i", y);
  Yboard.display.setCursor(85, 0);
  Yboard.display.printf("z:%i", z);

  //Draw initial switch states
  for (int i = 0; i < 4; ++i) {
    int sx = SWITCH_X + i * (SWITCH_WIDTH + PADDING);
    Yboard.display.drawRoundRect(sx, SWITCH_Y, SWITCH_WIDTH, SWITCH_HEIGHT, SWITCH_RADIUS, ON);
    if (sw[i]) {
      Yboard.display.fillCircle(sx + SWITCH_WIDTH / 2, SWITCH_Y + PADDING + SWITCH_RADIUS, SWITCH_RADIUS, ON);
    } else {
      Yboard.display.fillCircle(sx + SWITCH_WIDTH / 2, SWITCH_Y + SWITCH_HEIGHT - PADDING - SWITCH_RADIUS, SWITCH_RADIUS, ON);
    }
  }

  //Draw initial button states
  for (int i = 0; i < 5; ++i) {
    int bx = BUTTON_X + i * (BUTTON_WIDTH + PADDING);
    Yboard.display.drawRect(bx, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, ON);
    if (but[i]) {
      Yboard.display.fillCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, ON);
    } else {
      Yboard.display.drawCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, ON);
    }
  }

  //Yboard.display.drawCircle(KNOB_X, KNOB_Y, KNOB_RADIUS, ON);
  Yboard.display.display();
  
}

void loop() {
  // Test switches
  for(int i = 0; i < 4; ++i) {
    temp = Yboard.get_switch(i + 1);
    if (temp && !sw[i]) {
      sw[i] = 1;
      Yboard.set_led_color(26 - i, 255, 255, 255);
      Yboard.display.fillCircle(SWITCH_X + (SWITCH_WIDTH + PADDING) * i + SWITCH_WIDTH / 2, SWITCH_Y + SWITCH_HEIGHT - PADDING - SWITCH_RADIUS, SWITCH_RADIUS, OFF);
      Yboard.display.fillCircle(SWITCH_X + (SWITCH_WIDTH + PADDING) * i + SWITCH_WIDTH / 2, SWITCH_Y + PADDING + SWITCH_RADIUS, SWITCH_RADIUS, ON);
      Yboard.display.display();
    } else if (!temp && sw[i]) {
      sw[i] = 0;
      Yboard.set_led_color(26 - i, 0, 0, 0);
      Yboard.display.fillCircle(SWITCH_X + (SWITCH_WIDTH + PADDING) * i + SWITCH_WIDTH / 2, SWITCH_Y + PADDING + SWITCH_RADIUS, SWITCH_RADIUS, OFF);
      Yboard.display.fillCircle(SWITCH_X + (SWITCH_WIDTH + PADDING) * i + SWITCH_WIDTH / 2, SWITCH_Y + SWITCH_HEIGHT - PADDING - SWITCH_RADIUS, SWITCH_RADIUS, ON);
      Yboard.display.display();
    }
  }

  // Test buttons
  temp = Yboard.get_button(1);
  if (temp && !but[0]) {
    but[0] = 1;
    // Yboard.set_led_color(3, 255, 255, 255);
    Yboard.display.drawCircle(BUTTON_X+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, OFF);
    Yboard.display.fillCircle(BUTTON_X+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2-1, 6, ON);
    Yboard.display.display();
    Yboard.set_all_leds_color(255, 255, 255);
    
    Yboard.play_notes("T160 V2 O5 F D. C#8 D. C#8 D8 B8- O4 G8 F"); // Play fight song
    Yboard.set_all_leds_color(0, 0, 0);
  } else if (!temp && but[0]) {
    but[0] = 0;
    Yboard.set_all_leds_color(0, 0, 0);
    Yboard.display.fillCircle(BUTTON_X+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2-1, 6, OFF);
    Yboard.display.drawCircle(BUTTON_X+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, ON);
    Yboard.display.display();
  }

  temp = Yboard.get_button(2);
  if (temp && !but[1]) {
    but[1] = 1;
    Yboard.display.drawCircle(BUTTON_X+BUTTON_WIDTH+PADDING+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, OFF);
    Yboard.display.fillCircle(BUTTON_X+BUTTON_WIDTH+PADDING+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, ON);
    Yboard.display.display();
    Yboard.set_recording_volume(3);
    bool started_recording = Yboard.start_recording("/hardware_test.wav");
    while (Yboard.get_button(2)) {
        if (started_recording) {
            Yboard.set_all_leds_color(255, 0, 0);
            delay(100);
        } else {
            Yboard.set_all_leds_color(100, 100, 100);
            delay(100);
            Yboard.set_all_leds_color(0, 0, 0);
            delay(100);
        }
    }

    if (started_recording) {
        delay(200); // Don't stop the recording immediately
        Yboard.stop_recording();
        delay(100);
        Yboard.set_all_leds_color(0, 255, 0);
        Yboard.set_sound_file_volume(10);
        Yboard.play_sound_file("/hardware_test.wav");
        Yboard.set_all_leds_color(0, 0, 0);
        Yboard.set_sound_file_volume(5);
    }
  } else if (!temp && but[1]) {
    but[1] = 0;
    Yboard.set_all_leds_color(0, 0, 0);
    Yboard.display.fillCircle(BUTTON_X+BUTTON_WIDTH+PADDING+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, OFF);
    Yboard.display.drawCircle(BUTTON_X+BUTTON_WIDTH+PADDING+BUTTON_WIDTH/2, BUTTON_Y+BUTTON_HEIGHT/2, 6, ON);
    Yboard.display.display();
  }

  //Display button states for buttons 3-5
  for (int i = 2; i < 5; ++i) {
    int bx = BUTTON_X + i * (BUTTON_WIDTH + PADDING);
    bool pressed = Yboard.get_button(i + 1);
    if (pressed && !but[i]) {
      but[i] = 1;
      Yboard.display.drawCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, OFF);
      Yboard.display.fillCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, ON);
      Yboard.display.display();
    } else if (!pressed && but[i]) {
      but[i] = 0;
      Yboard.display.fillCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, OFF);
      Yboard.display.drawCircle(bx + BUTTON_WIDTH / 2, BUTTON_Y + BUTTON_HEIGHT / 2, 6, ON);
      Yboard.display.display();
    }
  }

  // Test Knob
  Yboard.display.fillCircle(knob_x, knob_y, 2, OFF);
  Yboard.display.fillCircle(KNOB_X, KNOB_Y, KNOB_RADIUS, OFF);
  knob = Yboard.get_knob();
  temp = map(knob % 120, 0, 120, 0, 255);
  Yboard.set_led_color(5, temp, temp, temp);
  
  knob_button_pressed = Yboard.get_knob_button();
  if(knob_button_pressed) {
    Yboard.display.fillCircle(KNOB_X, KNOB_Y, KNOB_RADIUS, ON);
  } else {
    Yboard.display.drawCircle(KNOB_X, KNOB_Y, KNOB_RADIUS, ON);
  }
  temp = map(knob % 30, 0, 30, 0, 360);
  knob_x = cos(temp * PI / 180 + PI / 4) * KNOB_RADIUS + KNOB_X;
  knob_y = sin(temp * PI / 180 + PI / 4) * KNOB_RADIUS + KNOB_Y;
  Yboard.display.fillCircle(knob_x, knob_y, 2, ON);

  // Test accelerometer
  if (Yboard.accelerometer_available()) {
    accelerometer_data accel_data = Yboard.get_accelerometer();
    temp = accel_data.x;
    if (temp != x) {
      x = temp;
      if (x > 999) {
        x = 999;
      } else if (x < -999) {
        x = -999;
      }
      if (x < 0) {
        Yboard.set_led_color(6, map(x, 0, -999, 0, 255), 0, 0);
      } else {
        Yboard.set_led_color(6, 0, map(x, 0, 999, 0, 255),
                           map(x, 0, 999, 0, 255));
      }
      Yboard.display.setCursor(10, 0);
      Yboard.display.printf("%04i", x);
    }

    temp = accel_data.y;
    if (temp != y) {
      y = temp;
      if (y > 999) {
        y = 999;
      } else if (y < -999) {
        y = -999;
      }
      if (y < 0) {
        Yboard.set_led_color(7, map(y, 0, -999, 0, 255), 0,
                            map(y, 0, -999, 0, 255));
      } else {
        Yboard.set_led_color(7, 0, map(y, 0, 999, 0, 255), 0);
      }
      Yboard.display.setCursor(42+10, 0);
      Yboard.display.printf("%04i", y);
    }

    temp = accel_data.z;
    if (temp != z) {
      z = temp;
      if (z > 999) {
        z = 999;
      } else if (z < -999) {
        z = -999;
      }
      if (z < 0) {
        Yboard.set_led_color(8, map(z, 0, -999, 0, 255),
                            map(z, 0, -999, 0, 255), 0);
      } else {
        Yboard.set_led_color(8, 0, 0, map(z, 0, 999, 0, 255));
      }
      Yboard.display.setCursor(85+10, 0);
      Yboard.display.printf("%04i", z);
    }
  }
  Yboard.display.display(); // Draw on display
}