/*
Code for crystal_ball.
Taken from the Arduino Uno Starter Kit.

By Leonardo Stefanini and Fabio Stefanini
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int switchPin = 6;
const int resetPin = 8;
const int piezoPin = 9;
int switchState = 0;
int prevSwitchState = 0;
int resetState = 0;
int prevResetState = 0;
int reply;

char to_write[16];
unsigned long lap_time = 0;
unsigned long old_time = 0;
unsigned long now = 0;
unsigned long old_now = 0;
const unsigned long MAX_TIME = 300000;  // millis
const unsigned long UPDATE_TIME = 10;  // millis
const unsigned long TOTAL_LAPS = 20;  // total laps
//unsigned long laps[TOTAL_LAPS];
unsigned long best_lap = 0;
unsigned long lap_total = 0;
unsigned long mean_lap = 0;
unsigned long n_laps = 0;

const unsigned long TIME_SEMAPHORE = 1000;  // millis

bool running = false;

/*void show(this_time, line) {
  lcd.setCursor(0, line)
  if this_time > 60000 {
    sprintf(to_write, "> 1min");
  }
  else {
    sprintf(to_write, "%02d:%02d", this_time*1e-3, (this_time%60000)*1e-3);
  }
  lcd.print(to_write);
}*/

void setup() {
    lcd.begin(16, 2);
    pinMode(switchPin, INPUT);
    pinMode(resetPin, INPUT);
    lcd.clear();
    lcd.setCursor(0, 0);
//    lcd.print("Lap   :");
    if (n_laps > 0) {
      mean_lap = 1.*lap_total/n_laps;
    }
    else {
      mean_lap = 0;
    }
    sprintf(to_write, "Mean: %2d.%02d s", int(mean_lap/1000), int((mean_lap%1000)/10));
    lcd.print(to_write);
//    lcd.print(n_laps);
//    lcd.print("/");
//    lcd.print(lap_total);
    lcd.setCursor(0, 1);
//    lcd.print("Time  :");
    sprintf(to_write, "Best: %2d.%02d s", int(best_lap/1000), int((best_lap%1000)/10));
    lcd.print(to_write);
    running = false;
    n_laps = 0;
    lap_total = 0;
    best_lap = 0;
}

void loop() {
    switchState = digitalRead(switchPin);
    resetState = digitalRead(resetPin);

    // reset
    if (resetState != prevResetState and resetState == HIGH) {
      setup();
    }

    // lap
    if (switchState != prevSwitchState and switchState == HIGH) {
      updateLap();
    }

    // update time
    now = millis();
    if (now-old_now>UPDATE_TIME and running) {
      updateTime();
    }

    // reset if too long
    if (now-old_now>MAX_TIME or n_laps>=TOTAL_LAPS) {
      setup();
      old_now = now;
    }
    
    prevSwitchState = switchState;
    prevResetState = resetState;
}

void semaphore() {
  lcd.clear();
  // semaphore
  delay(TIME_SEMAPHORE);
  lcd.setCursor(1, 0);
  lcd.print("**");
  lcd.setCursor(1, 1);
  lcd.print("**");
  delay(TIME_SEMAPHORE);
  lcd.setCursor(1, 0);
  lcd.print("** **");
  lcd.setCursor(1, 1);
  lcd.print("** **");
  delay(TIME_SEMAPHORE);
  lcd.setCursor(1, 0);
  lcd.print("** ** **");
  lcd.setCursor(1, 1);
  lcd.print("** ** **");
  delay(TIME_SEMAPHORE);
  lcd.setCursor(1, 0);
  lcd.print("** ** ** **");
  lcd.setCursor(1, 1);
  lcd.print("** ** ** **");
  delay(TIME_SEMAPHORE);
  lcd.setCursor(1, 0);
  lcd.print("** ** ** ** **");
  lcd.setCursor(1, 1);
  lcd.print("** ** ** ** **");
  delay(TIME_SEMAPHORE);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lap   :");
  lcd.setCursor(8, 0);
  lcd.print("  0.00 s ");
  lcd.setCursor(5, 0);
  lcd.print("0");
  lcd.setCursor(0, 1);
  lcd.print("Time  :");
}

void updateLap() {
  now = millis();
  lap_time = now - old_time;
  lcd.setCursor(9, 0);
  if (lap_time >= 60000 and running) {
    lcd.print("> 1m  ");
    lap_total += lap_time;
  }
  if (lap_time < 60000 and running) {
    sprintf(to_write, "%2d.%02d s", int(lap_time/1000), int((lap_time%1000)/10));
    lcd.print(to_write);
    n_laps += 1;
    lcd.setCursor(4, 0);
    sprintf(to_write, "%2d", n_laps);
    lcd.print(to_write);
//        laps[n_laps-1] = lap_time;
    if (best_lap == 0 or lap_time<best_lap) {
      best_lap = lap_time;
    }
    lap_total += lap_time;
  }
  
  if (not running) {
    running = true;
    semaphore();
    now = millis();
    old_now = now;
  }

  if (1 <= (TOTAL_LAPS-n_laps) <= 5) {
    tone(piezoPin, 1000, 100);
  }

  if ((TOTAL_LAPS-n_laps) == 0) {
    tone(piezoPin, 5000, 500);
  }
  
  old_time = now;
}

void updateTime() {
  lcd.setCursor(7, 1);
  sprintf(to_write, "%4d.%02d s", int((now-old_time)/1000), int(((now-old_time)%1000)/10));
  lcd.print(to_write);
  old_now = now;
}
