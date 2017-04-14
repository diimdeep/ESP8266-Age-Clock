const char activeSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000
};

const char inactiveSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
};

#define wifi_01_width 8
#define wifi_01_height 8
static unsigned char wifi_01_bits[] = {
 0x80, 0xA0, 0xA8, 0xAB, 0xAB, 0xA8, 0xA0, 0x80 };

#define wifi_02_width 8
#define wifi_02_height 8
const char wifi_02_bits[] PROGMEM = {
 0x40, 0x90, 0xA4, 0xA9, 0xA9, 0xA4, 0x90, 0x40 };
