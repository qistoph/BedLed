#include "quake.h"
#include "lightcontrol.h"

// 0 normal
const char lightStyle0[] PROGMEM = "m";

// 1 FLICKER (first variety)
const char lightStyle1[] PROGMEM = "mmnmmommommnonmmonqnmmo";

// 2 SLOW STRONG PULSE
const char lightStyle2[] PROGMEM = "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba";

// 3 CANDLE (first variety)
const char lightStyle3[] PROGMEM = "mmmmmaaaaammmmmaaaaaabcdefgabcdefg";

// 4 FAST STROBE
const char lightStyle4[] PROGMEM = "mamamamamama";

// 5 GENTLE PULSE 1
const char lightStyle5[] PROGMEM = "jklmnopqrstuvwxyzyxwvutsrqponmlkj";

// 6 FLICKER (second variety)
const char lightStyle6[] PROGMEM = "nmonqnmomnmomomno";

// 7 CANDLE (second variety)
const char lightStyle7[] PROGMEM = "mmmaaaabcdefgmmmmaaaammmaamm";

// 8 CANDLE (third variety)
const char lightStyle8[] PROGMEM = "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa";

// 9 SLOW STROBE (fourth variety)
const char lightStyle9[] PROGMEM = "aaaaaaaazzzzzzzz";

// 10 FLUORESCENT FLICKER
const char lightStyle10[] PROGMEM = "mmamammmmammamamaaamammma";

// 11 SLOW PULSE NOT FADE TO BLACK
const char lightStyle11[] PROGMEM = "abcdefghijklmnopqrrqponmlkjihgfedcba";


const char * const lightstyles[] PROGMEM = {
	lightStyle0,
	lightStyle1,
	lightStyle2,
	lightStyle3,
	lightStyle4,
	lightStyle5,
	lightStyle6,
	lightStyle7,
	lightStyle8,
	lightStyle9,
	lightStyle10,
	lightStyle11,
};

uint8_t quake_mode = 0;
uint8_t max_quake_mode = 12;

const char *quake_seq_addr;
unsigned int quake_seq_l = 0;

uint8_t nextQuake() {
	quake_mode = (quake_mode + 1) % max_quake_mode;
	//quake_seq = lightstyles[quake_mode];

	quake_seq_addr = (char*)pgm_read_word(&(lightstyles[quake_mode]));

	quake_seq_l = strlen_P(quake_seq_addr);
	MySerial.print(F("Quake len: "));
	MySerial.println(quake_seq_l);
	return quake_mode;
}

void lightQuake() {
    unsigned int n = (millis() / FTIME) % quake_seq_l;

	char q = pgm_read_byte(quake_seq_addr + n);

	//MySerial.print(q);

    int value = q - 'a'; //Get the value from a to z from the string array minus 'a' (ASCII 0x61). This returns a value from 0 to 26.
    value = map(value, 0, 25, 0, 255); // convert from 0-25 to 0-255
    lightSetDimm(value);
}
