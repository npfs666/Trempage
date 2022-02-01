/**
 * lcd.h
 *
 * Drivers LCD pour modules standarts 1x8, 2x16, 2x20....
 * Mode 4 bits DATA + E + RS
 *
 * Développé sur MPLABX pour les compilateurs XC
 *
 * Auteur : Brendan Jaouen
 *
 * Date : Janvier 2017
 */

#define NB_ROW 2
#define NB_COL 16



/**
 *
 * Prototypes
 *
 */
void initLCD( void );
void sendNibble( uint8_t data );
void setInstruction( uint8_t cmde );
void setData( int8_t data );
void setStr(char *string);
void clrLcd(void);
uint8_t setCursorAt(uint8_t col, uint8_t row);
uint8_t setStrAt(uint8_t col, uint8_t row, char* str);
void setBlinkOn(void);
void setBlinkOff(void);