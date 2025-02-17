#include "LedControl.h"

LedControl lc=LedControl(12,10,11,1);
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
int delayMultiplier = 3;
int x = 0;
int y = 0;
int dir = 1;
int pos[2] {0, 0};
int applePos[2] {0, 0};
int snake[63] {0};
int matrix[8][8] {{0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0}};

void clearMatrix(){
  for (int row = 0; row < 8; row++){
    for (int column = 0; column < 8; column++){
      matrix[row][column] = 0;
    }
  }
}

void resetSnake(){
  pos[0] = 0;
  pos[1] = 0;
  for (int i = 0; i < 63; i++){
    snake[i] = 0;
  }
  dir = 1;
}

void showMatrix(){
  byte row;
  for (int i = 0; i < 8; i++){
    row = convertRowToByte(i);
    lc.setRow(0, i, row);
  }
}

void resetApple(){
  int rows[8] {0};
  int next = 0;
  for (int i = 0; i < 8; i++){
    if (convertRowToByte(i) != B11111111){
      rows[next] = i;
      next++;
    }
  }
  int rowNum = rows[rand() % next];
  int elems[8] {0};
  next = 0;
  for (int i = 0; i < 8; i++){
    if (matrix[rowNum] != 0){
      elems[next] = i;
      next++;
    }
  }
  int elemNum = elems[rand() % next];
  applePos[0] = rowNum;
  applePos[1] = elemNum;
}

bool isApple(){
  return pos[0] == applePos[0] && pos[1] == applePos[1];
}

void appleCycle(){
  for (int i = 0; i < 2; i++){
  matrix[applePos[0]][applePos[1]] = (matrix[applePos[0]][applePos[1]] + 1) % 2;
  showMatrix();
  delay(delayMultiplier * 25);
  }
}

byte convertRowToByte(int rowIndex){
  byte ret = B0;
  int row[8] {0};
  for (int i = 0; i < 8; i++){
    ret = ret * 2 + matrix[rowIndex][i];
  }
  return ret;
}

void reduceSensitivity(){
  if (-100 < x && x < 100) {
    x = 0;
  }
  if (-100 < y && y < 100) {
    y = 0;
  }
}

void getDirection(){
  x = analogRead(X_pin) - 512;
  y = analogRead(Y_pin) - 512;
  reduceSensitivity();

  if (x < y) {
    if (-x < y) {
      dir = 2;
    }
    else if (y < -x){
      dir = 1;
    }
  }
  else if (y < x) {
    if (x < -y) {
      dir = 4;
    }
    else if (-y < x){
      dir = 3;
    }
  }
}

void gameOver(){
  resetSnake();
}

void turn(){
  int off = 0;
  x = pos[0];
  y = pos[1];
  clearMatrix();
  bool legal = true;
  matrix[x][y] = 1;
  for (int i = 0; i < 63 && snake[i] != 0; i++){
    off = snake[i];
    if (off  % 2 == 1){
      x =  (x + off + 6) % 8;
    }
    else{
      y = (y + off + 5) % 8;
    }
    matrix[x][y] = 1;
  }
  appleCycle();

  getDirection();
  pos[(dir - 1) % 2] = (pos[(dir - 1) % 2] + 3 - dir - dir % 2 + 8) % 8;

  if (matrix[pos[0]][pos[1]] == 0){
    int i = 0;
    while (i < 63 && snake[i] != 0){
      i++;
    }
  
    if (!isApple()){
      i--;
    }
  
    while (0 < i){
      snake[i] = snake[i-1];
      i--;
    }
  
    if (snake[0] != 0 || isApple()){
      snake[0] = dir;
    }
  
    if (isApple()){
      resetApple();
    }
  }else{
    gameOver();
  }
}

void setup() {
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  resetApple();
}

void checkSpeed(){
  if (digitalRead(SW_pin) == 0){
    delayMultiplier = (delayMultiplier * 2 - 1) % 15 + 1;
  }
}

void loop() {
  Serial.print("Direction:  ");
  Serial.print(dir);
  Serial.print("\n\n");
  turn();
  checkSpeed();
}
