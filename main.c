/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>


#include "trempage.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include <libpic30.h>


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{

    /* Configure the oscillator for the device */
    ConfigureOscillator();

    // Délai avant réel démarrage du soft
    __delay_ms(100);
    
    /* Initialize IO ports and peripherals */
    InitApp();
    
    // Récupération des données dans l'EEPROM
    retrieveEEPROMdata();

    // Si cycle interrompu en cours, relancer le process
    if( getEtat() == 1 ) {
        repriseCycle();
    } else {
        afficherAccueil();
    }
    
    // on provoque une IT bouton pour mettre à jour l'état (typ: auto/man)
    _CNIF = 1;
    
    Idle();
    while(1);
}
