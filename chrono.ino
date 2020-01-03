/*
Adapted from the crystal_ball example code in the Arduino Uno Starter Kit.
By Leonardo Stefanini and Fabio Stefanini
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// total time before forced reset
const unsigned long MAX_TIME = 60000;  // millis
// update clock time
const unsigned long UPDATE_TIME = 10;  // millis
// scroll time
const unsigned long SCROLL_TIME = 2000;  // millis
// avoid double signaling of a lap
const unsigned long LAP_TIME_THRESHOLD = 1000;  // millis
// time between lights in the semaphore
const unsigned long TIME_SEMAPHORE = 1000;  // millis
// typical lap time (Carrera Go! -> 2700)
const unsigned long TYPICAL_LAP_TIME = 2700;  // millis
// typical lap time tolerance
const unsigned long TYPICAL_LAP_TIME_TOL = 2000;  // millis
// total laps
int TOTAL_LAPS = 20;
// last laps
int LAST_LAPS = 5;
// tones
int TONE0 = 100;      // semaphore
int TONE0_DUR = 100;  // duration, millis
int TONE1 = 500;      // lap
int TONE1_DUR = 50;
int TONE2 = 1000;     // last laps
int TONE2_DUR = 50;
int TONE3 = 2500;     // race start and end
int TONE3_DUR = 500;

// pins of the sensors
const int switchPin[2] = {6, 7};
const int resetPin = 8;
const int piezoPin = 9;
int switchState[2] = {0, 0};
int prevSwitchState[2] = {0, 0};
int resetState = 0;
int prevResetState = 0;
int summaryVar[2] = {0, 0};

char to_write[16];
unsigned long lap_time[2];
unsigned long old_time[2];
unsigned long now;
unsigned long old_now[2];
unsigned long best_lap[2];
unsigned long lap_total[2];
unsigned long mean_lap[2];
int n_laps[2] = {-1, -1};
unsigned long now_scroll[2] = {0, 0};

bool running[2] = {false, false};

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
    pinMode(switchPin[0], INPUT);
    pinMode(switchPin[1], INPUT);
    pinMode(resetPin, INPUT);
    summary(0);
    summary(1);
}

void loop() {
  resetState = digitalRead(resetPin);
  now = millis();
  // reset
  if (resetState != prevResetState and resetState == HIGH) {
    if (running[0] or running[1]) {
      setup();
      running[0] = false;
      running[1] = false;
    }
    else {
      running[0] = true;
      running[1] = true;
      resetLaps(0);
      resetLaps(1);
      semaphore();
      now = millis();
      old_now[0] = now;
      old_time[0] = now;
      old_now[1] = now;
      old_time[1] = now;
    }
  }
    
  for (int i=0; i<2; ++i) {
    switchState[i] = digitalRead(switchPin[i]);

    if (running[i]) {
      // lap
      if (switchState[i] != prevSwitchState[i] and switchState[i] == HIGH) {
        lap_time[i] = now - old_time[i];
        if (lap_time[i] >= LAP_TIME_THRESHOLD) {
          updateLap(i);
        }
      }
  
      // update time
      if (now-old_now[i]>UPDATE_TIME) {
        updateTime(i);
        old_now[i] = now;
      }

      /*
      // reset if too long
      if (now-old_now>MAX_TIME or n_laps>=TOTAL_LAPS) {
        setup();
        old_now = now;
      }*/
      if (now-old_now[i]>MAX_TIME or n_laps[i]>=TOTAL_LAPS) {
        summary(i);
        old_now[i] = now;
        running[i] = false;
      }
    }
      // scroll
    else {
      if (now-now_scroll[i]>SCROLL_TIME) {
        summary(i);
        now_scroll[i] = now;
      }
    }
    prevSwitchState[i] = switchState[i];
  }    

  
  prevResetState = resetState;

}

void semaphore() {
  lcd.clear();
  // semaphore
  for (int i=0; i<6; ++i) {
    delay(TIME_SEMAPHORE);
    lcd.setCursor(1+i*3, 0);
    lcd.print("**");
    lcd.setCursor(1+i*3, 1);
    lcd.print("**");
    tone(piezoPin, TONE0, TONE0_DUR);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.setCursor(0, 1);
  lcd.print("R:");
  tone(piezoPin, TONE3, TONE3_DUR);
}

void updateLap(int i) {

  lcd.setCursor(7, i);

  // valid lap
  if (lap_time[i]<(TYPICAL_LAP_TIME+TYPICAL_LAP_TIME_TOL)) {      
    sprintf(to_write, "%2d.%02ds", int(lap_time[i]/1000), int((lap_time[i]%1000)/10));
    lcd.print(to_write);
    n_laps[i] = n_laps[i]+1;
    lcd.setCursor(13, i);
    sprintf(to_write, "/%2d", n_laps[i]);
    lcd.print(to_write);
    if (best_lap[i] == 0 or lap_time[i]<best_lap[i]) {
      best_lap[i] = lap_time[i];
    }
    lap_total[i] += lap_time[i];
  }
  else {
    sprintf(to_write, "  -inv-  ");
    lcd.print(to_write);
  }
  
  if (TOTAL_LAPS-n_laps[i]>LAST_LAPS and n_laps[i]>0) {
    tone(piezoPin, TONE1, TONE1_DUR);
  }
  
  if (TOTAL_LAPS-n_laps[i] <= LAST_LAPS) {
    tone(piezoPin, TONE2, TONE2_DUR);
  }
  
  if ((TOTAL_LAPS-n_laps[i]) == 0) {
    tone(piezoPin, TONE3, TONE3_DUR);
  }
  
  old_time[i] = now;
  
}

void updateTime(int i) {
  lcd.setCursor(2, i);
  sprintf(to_write, "%2d.%01d", int((now-old_time[i])/1000), int(((now-old_time[i])%1000)/100));
  lcd.print(to_write);
}

void summary(int i) {
    lcd.setCursor(0, i);
    if (i==0) {
      lcd.print("L: ");
    }
    else {
      lcd.print("R: ");
    }
    lcd.setCursor(3, i);
    if (n_laps > 0) {
      mean_lap[i] = 1.*lap_total[i]/n_laps[i];
    }
    else {
      mean_lap[i] = 0;
    }
    if (summaryVar[i] == 0) {
      sprintf(to_write, "mean %2d.%02ds  ", int(mean_lap[i]/1000), int((mean_lap[i]%1000)/10));
    }
    if (summaryVar[i] == 1) {
      sprintf(to_write, "best %2d.%02ds  ", int(best_lap[i]/1000), int((best_lap[i]%1000)/10));
    }
    if (summaryVar[i] == 2) {
      sprintf(to_write, "tot %3d.%02ds  ", int(lap_total[i]/1000), int((lap_total[i]%1000)/10));
      summaryVar[i] = -1;
    }
    lcd.print(to_write); 
    summaryVar[i] += 1;   
}

void resetLaps(int i) {
  n_laps[i] = -1;
  lap_total[i] = 0;
  best_lap[i] = 0;
}
