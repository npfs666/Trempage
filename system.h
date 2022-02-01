/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        8000000L    // Clock @ 2MHz
#define FCY             SYS_FREQ/2  // = 1 Mips

/* Entrées */

    // Commandes manuelles
    #define BOUTON_MAN_AUTO     PORTBbits.RB13
    #define BOUTON_REMPLISSAGE  PORTBbits.RB14
    #define BOUTON_VIDANGE      PORTBbits.RB15

    // Pic
    #define BOUTON_CENTRAL        PORTBbits.RB9
    //#define ENCA              PORTBbits.RB12
    #define ENCB                PORTBbits.RB8
    #define BOUTON_RESET_EEP    PORTBbits.RB12

/* Sorties */

    // LCD
    #define D4      LATBbits.LATB0
    #define D5      LATBbits.LATB1
    #define D6      LATBbits.LATB2
    #define D7      LATBbits.LATB3

    #define RS      LATAbits.LATA0
    #define E       LATAbits.LATA1

    // Electrovannes
    #define EV_REMPLIR      LATBbits.LATB7
    #define EV_VIDER        LATBbits.LATB6

    // Voyants d'états
    #define LED_EN_COURS    LATBbits.LATB5
    #define LED_PAUSE       LATAbits.LATA3
    #define LED_FIN         LATAbits.LATA2

 

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/



void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
int16_t readEEP(const uint16_t adress);
void writeEEP(uint16_t address, int16_t data);
uint16_t getRTCCDay();
