#define LATCH_DIO 4
#define DATA_DIO 8
#define CLK_DIO 7
const byte SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0b10011000};//0x90
const byte SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

enum stateEnum {
  up,
  down
};

stateEnum state;

byte level = 0;
int timerCounter = 0;

void setup() {

  state = up;

  pinMode(LATCH_DIO, OUTPUT);
  pinMode(DATA_DIO, OUTPUT);
  pinMode(CLK_DIO, OUTPUT);

  pinMode(A1, INPUT);

}

void loop() {

  timerCounter = 0;

  while (digitalRead(A1) == 1);
  while (digitalRead(A1) == 0);

  switch (state) {

    case up:
      torUp();
      state = down;
      break;

    case down:
      torDown();
      state = up;
      break;

  }

}


void torUp() {

  while (true) {

    showLevel();
    delay(1);
    timerCounter++;

    if (level == 99){
      showTorStop();
      break;
    }
      

    if(digitalRead(A1)==0){
      while (digitalRead(A1) == 0);
      showTorStop();
      break;
    }
      

    if (timerCounter == 500) {
      level++;
      timerCounter = 0;
    }


  }

}

void torDown() {

  while (true) {

    showLevel();
    delay(1);
    timerCounter++;

    if (level == 0){
      showTorStop();
      break;
    }

    if(digitalRead(A1)==0){
      showTorStop();
      while (digitalRead(A1) == 0);
      break;
    }

    if (timerCounter == 500) {
      level--;
      timerCounter = 0;
    }

  }
}



void showTorStop(){
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, 0b10111111);
  delay(1);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[0]);
  digitalWrite(LATCH_DIO, HIGH);
}



void showLevel() {

  byte first = level / 10;
  byte second = level % 10;
  
  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[first]);
  delay(1);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[0]);
  digitalWrite(LATCH_DIO, HIGH);

  digitalWrite(LATCH_DIO, LOW);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[second]);
  delay(1);
  shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[1]);
  digitalWrite(LATCH_DIO, HIGH);

  
}
