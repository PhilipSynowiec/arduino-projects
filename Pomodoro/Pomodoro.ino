#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

int session = 0;
int seconds = 0;
bool working = false;
void changeState(){
  working = (working + 1) % 2;
  lcd.clear();
  lcd.setCursor(0, 0);
  if (working){
    session++;
    seconds = 1500;
    lcd.print("Work!");
  }
  else {
    seconds = 300;
    lcd.print("Break!");
  }
  lcd.setCursor(0, 1);
  lcd.print("Session: ");
  lcd.print(session);
}

void displayTime(){
  lcd.setCursor(11, 0);
  int rsecs = seconds % 60;
  int mins = (seconds - rsecs) / 60;
  if (mins == mins % 10){
    lcd.print(0);
  }
  lcd.print(mins);
  lcd.print(":");
  if (rsecs == rsecs % 10){
    lcd.print(0);
  }
  lcd.print(rsecs);
}

void setup() {
  lcd.begin(16, 2);
  pinMode(13, OUTPUT);
}

int backup = 0;
void loop() {
  backup = seconds;
  if (seconds == 0){
    changeState();
    digitalWrite(13, HIGH);
  }
  displayTime();
  delay(1000);
  seconds -= 1;
  if (backup == 0){
    digitalWrite(13, LOW);
  }
}
