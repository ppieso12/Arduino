//#include <TM1638.h>
#define DIGIT_0 0x3f
#define DIGIT_1 0x06
#define DIGIT_2 0x5b
#define DIGIT_3 0x4f
#define DIGIT_4 0x66
#define DIGIT_5 0x6d
#define DIGIT_6 0x7d
#define DIGIT_7 0x07
#define DIGIT_8 0x7f
#define DIGIT_9 0x6f
#define DIGIT_A 0x77
#define DIGIT_b 0x7c
#define DIGIT_C 0x39
#define DIGIT_d 0x5E
#define DIGIT_E 0x79
#define DIGIT_F 0x71
//dla errorów
#define DIGIT__ 0x40
#define DIGIT_r 0x50
#define DIGIT_o 0x5c
#define DIGIT_P 0x73
#define DIGIT_S 0x6D
#define DIGIT_I 0x30

const int stb = 10;
const int clk = 9;
const int dio = 8;
const byte error_code[] = {DIGIT_E, DIGIT_r, DIGIT_r, DIGIT__, DIGIT_C, DIGIT_o, DIGIT_d, DIGIT_E};
const byte error_times[] = {DIGIT_E, DIGIT_r, DIGIT_r, DIGIT__, DIGIT_r, DIGIT_E, DIGIT_P, DIGIT_S};
const byte error_disp[] = {DIGIT_E, DIGIT_r, DIGIT_r, DIGIT__, DIGIT_d, DIGIT_I, DIGIT_S, DIGIT_P}; 
/*TO DO
Program powinien sygnalizować (poprzez wyświetlenie odpowiedniego komunikatu) 
ewentualne błędy w podanych komendach lub parametrach. 
*/
 
byte digits[115];
byte text[8];   //docelowy text wypisywany w funkcji printText

char CODE[8]="EbF12375";  //Domyslny / zmienny przez kompa
int TIMES = 1;
int DISP = 100;

int last_i = 0;
int last_n = 0;
int i = 0;
int n = 0;

bool completed = false;
bool wait = false;
 
void sendCommand(byte value){
  digitalWrite(stb,LOW);
  shiftOut(dio,clk,LSBFIRST,value);
  digitalWrite(stb,HIGH);
}
 
void reset(){
  sendCommand(0x40);
  digitalWrite(stb,LOW);
  shiftOut(dio,clk,LSBFIRST,0xc0);
  for(byte i=0; i<16; i++){
    shiftOut(dio,clk,LSBFIRST,0x00);
  }
  digitalWrite(stb,HIGH);
}
 
 
void setup() {
  pinMode(stb, OUTPUT);
  pinMode(clk,OUTPUT);
  pinMode(dio, OUTPUT);
  init();
  read_code();
  sendCommand(0x8F);
  reset();
}
 
void loop() {
 // last_n = 0;
 // last_i = 0;
  action_listener(pressedButton());
  
  if(!wait){
    reset();
   
    
    if(!completed){
   //   reset();

      for ( i = last_n; i<8; i++){
        for ( n = last_i; n<16*TIMES; n++){
         action_listener(pressedButton());
         if(wait){                //To last n i last i żeby mozna było startować od pewnego miejsca, niezbyt dobry pomysl
           last_n = n;
           break;
         }
          changing(i);
         
        }
         if(wait){
           last_i = i;
           break;
         }
         printText(i);
         lightsOn(i);
      }
      last_n = 0;
      last_i = 0;
      completed = true;
      wait = true;
      delay(1000);
    }
  }
}
 
bool printText(byte i){
 
  for(byte position = 0; position < 8; position++)
  {
    sendCommand(0x44);
    digitalWrite(stb, LOW);
    shiftOut(dio, clk, LSBFIRST, 0xC0 + (position << 1));
    shiftOut(dio, clk, LSBFIRST, text[position]);
    digitalWrite(stb, HIGH);
  }
}
 
bool changing(byte i){
  byte board[] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7,
                  DIGIT_8, DIGIT_9, DIGIT_A, DIGIT_b, DIGIT_C, DIGIT_d, DIGIT_E, DIGIT_F, };
  byte position = i;
  static byte index = 0;
  sendCommand(0x40);
  digitalWrite(stb,LOW);
  shiftOut(dio, clk, LSBFIRST, 0xc0 + (position << 1));
  for (position; position < 8; position++){
    shiftOut(dio, clk, LSBFIRST, board[index]);
    shiftOut(dio, clk, LSBFIRST, 0x00);
  }
  digitalWrite(stb,HIGH);
  delay(DISP);
  index = ++index % 16;
  return index == 0;
}
 
void lightsOn(byte i){
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0xc1+(i*2)); // led on
  shiftOut(dio, clk, LSBFIRST, 1);
  digitalWrite(stb, HIGH);
}

uint8_t readButtons(void)
{
  uint8_t buttons = 0;
  digitalWrite(stb, LOW);
  shiftOut(dio, clk, LSBFIRST, 0x42);
 
  pinMode(dio, INPUT);
 
  for (uint8_t i = 0; i < 4; i++)
  {
    uint8_t v = shiftIn(dio, clk, LSBFIRST) << i;
    buttons |= v;
  }
 
  pinMode(dio, OUTPUT);
  digitalWrite(stb, HIGH);
  return buttons;
}

int pressedButton(){
  uint8_t buttons = readButtons();
 
  for(uint8_t position = 0; position < 2; position++)
  {
    uint8_t mask = 0x1 << position;
    
    //https://blog.3d-logic.com/2015/01/10/using-a-tm1638-based-board-with-arduino/
    //zamiast tego -> setLed(buttons & mask ? 1 : 0, position); dałem tego ifa
    if (buttons & mask) {
      return position + 1;
    }else
      return 0;
  }
}

void action_listener( int button_number){
  
  if(button_number == 1){ //akcja dla 1 buttona
      if(wait){
        wait = false
      }else{
        wait = true;
      }                         /* Naciśnięcie w 
                                trakcie procesu łamania kodu przycisku S1 powoduje wstrzymanie łamania, a kolejne 
                                naciśnięcie wznawia łamanie*/
    
    
  }else if(button_number == 2){  // naciśnięcie przycisku S2 – wtedy ponownie łamany jest dotychczasowy kod 
    completed = false;          /*naciskanie przycisku S2 w trakcie łamania nie powinna powodować żadnej akcji. */
    wait = false;
  }
}

void read_code(){
 if(strlen(CODE) == 8){
  
   for(int i = 0; i < 8; i++){
     text[i] = digits[(int)(CODE[i])];
   }
 }else{
  printError(error_code);
  //error
 }
}

void init(){
  digits[48] = DIGIT_0;
  digits[49] = DIGIT_1;
  digits[50] = DIGIT_2;
  digits[51] = DIGIT_3;
  digits[52] = DIGIT_4;
  digits[53] = DIGIT_5;
  digits[54] = DIGIT_6;
  digits[55] = DIGIT_7;
  digits[56] = DIGIT_8;
  digits[65] = DIGIT_A;
  digits[98] = DIGIT_b;
  digits[67] = DIGIT_C;
  digits[100] = DIGIT_d;
  digits[69] = DIGIT_E;
  digits[70] = DIGIT_F;
}

void printError(byte *board){
  reset();
  wait = true;
  last_n = n;
  last_i = i;
  int position;
 
  sendCommand(0x40);
  digitalWrite(stb,LOW);
 
  for (position = 0; position < 8; position++){
    shiftOut(dio, clk, LSBFIRST, 0xc0 + (position << 1));
    shiftOut(dio, clk, LSBFIRST, board[position]);
    shiftOut(dio, clk, LSBFIRST, 0x00);
  }
  digitalWrite(stb,HIGH);
}
