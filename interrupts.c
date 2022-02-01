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

#include <stdint.h>        /* Includes uint16_t definition */
#include <stdbool.h>       /* Includes true/false definition */

#include "lcd4bit.h"
#include "system.h"
#include "user.h"
#include "trempage.h"
#include <libpic30.h>
#include <stdio.h>



/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* Curseur menu
 * 0 -> Nombre cycles; 1 -> Cycle 1; 2 -> Cycle 2; N -> Cycle N
 */
char cur_menu = 0;

// Buffer pour l'affichage sur le LCD
char affichage[20];



/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/



/**
 * Gestion de l'IT du RTCC
 */
void __attribute__((interrupt,no_auto_psv)) _RTCCInterrupt(void) {
    
    if( getEtat() == 1 ) {
        setCycleMinutes(getCycleMinutes() + 1);
        regulation();
    }
    
    ClrWdt();
    _RTCIF = 0;
    Idle();
}



/**
 * gestion de l'IT des boutons
 */
void __attribute__((interrupt,no_auto_psv)) _CNInterrupt(void) {
    
    uint16_t temps;
    
    __delay_ms(10);     // Anti rebond

    if( BOUTON_CENTRAL == 0 ) {

        // Menu réglage durée par cycle
        if( cur_menu < getCyclesMax() ) {
            
            // remplir les cycles
            clrLcd();
            sprintf(affichage, "   Cycle %2d : ", cur_menu+1);
            setStrAt(0,0,affichage);
            temps = getCycleN(cur_menu) * 10;
            sprintf(affichage, "    %2dh%2dm", temps/60, temps%60);
            setStrAt(0,1,affichage);
            
            // Si on modifie un prog en cours on met en pause
            if( getEtat() == 1 ) {
                setEtat(4);
                regulation();
                setLedPause();
            }            
            cur_menu++;
        }
        // Reset du programme (nombre de cycles @ 0 )
        else if( (cur_menu == 0) && (getCyclesMax() == 0) ) {
            
            setEtat(0);
            regulation();
            setEEPData();
            setLedFin();
        }
        // Fin de reconfiguration d'un programme en cours
        else if( (cur_menu >= getCyclesMax()) && (getEtat() == 4) ) {
            
            cur_menu = 0;
            setEtat(1);
            repriseCycle();
            setEEPData();
            setLedEnCours();
        }
        // Fin de config du cycle initial
        else if( (cur_menu >= getCyclesMax()) && (getEtat() != 1) ) {

            cur_menu = 0;
            setEtat(1);
            setCycleMinutesIndex(getRTCCDay());
            setCycleMinutes(0);
            setCycleActuel(0);
            afficherAccueil();
            remplir(1);
            setEEPData();
            setLedEnCours();
        }
    }
    
    // Bouton reset EEPROM
    else if( BOUTON_RESET_EEP == 0 ) {
        
        resetEEPData();
        LED_EN_COURS = 1;
        LED_FIN = 1;
        LED_PAUSE = 1;
    }
    
    // Passage en mode manuel
    else if( BOUTON_MAN_AUTO == 0 ) {
        if( getEtat() == 1 ) {
            // mise en pause
            setEtat(3);
            regulation();
            setLedPause();
        }
        
    }
    
    // passage en mode auto
    else if( BOUTON_MAN_AUTO == 1 ) {

        if( getEtat() == 3 ) {
            // remise en marche
            setEtat(1);
            repriseCycle();
            setLedEnCours();
        } 
        else if( getEtat() != 1 ) {
            EV_REMPLIR = 0;
            EV_VIDER = 0;
        }
    }
 
    
    // Boutons du mode manuel
    if( BOUTON_REMPLISSAGE == 0 ) {
        
        // Si mode manuel, on possibilité de jouer avec les boutons
        if ( (BOUTON_MAN_AUTO == 0) && (EV_VIDER == 0) )
            EV_REMPLIR = 1;
    }
    else {
        if ( BOUTON_MAN_AUTO == 0 )
            EV_REMPLIR = 0;
    }
    if( BOUTON_VIDANGE == 0 ) {
        
        // Si mode manuel, on possibilité de jouer avec les boutons
        if ( (BOUTON_MAN_AUTO == 0) && (EV_REMPLIR == 0) )
            EV_VIDER = 1;
    }
    else {
            if ( BOUTON_MAN_AUTO == 0 )
            EV_VIDER = 0;
    }
    

    //__delay_ms(50);     // Anti rebond
    ClrWdt();
    _CNIF = 0;
    Idle();
}



/**
 * Gestion de l'IT de IC3 -> Encodeur rotatif
 */
void __attribute__((interrupt,no_auto_psv)) _IC1Interrupt(void) {

    int16_t var;
 
    // Menu nombre de cycles
    if ( cur_menu == 0 ) {
        
        var = getCyclesMax();
        
        if( ENCB == 1 )
            var += 2;
        else
            var -= 2;

        if (var > (NB_MAX_CYCLES-2))
            var = 0;
        else if( var < 0 )
            var = NB_MAX_CYCLES-2;

        setCyclesMax(var);
        
        if( getEtat() == 1 ) {
            setEtat(4);
            regulation();
            setLedPause();
            clrLcd();
            setStrAt(0,0,"Nombre de cycles : ");
        }
        sprintf(affichage, "%2d", var);
        setStrAt(6, 1, affichage);
        
    }        
    // Menu durée cycle
    else if ((cur_menu <= getCyclesMax()) && (cur_menu != 0)) {

        var = getCycleN(cur_menu - 1);
        
        if( ENCB == 1 )
            var++;
        else
            var--;
        
        if (var > 98)
            var = 1;
        else if( var < 1 )
            var = 98;

        setCycleN(cur_menu - 1, var);  
        var *= 10;

        sprintf(affichage, "    %2dh%2dm", var/60, var%60);
        setStrAt(0, 1, affichage);
    }
    
    ClrWdt();
    _IC1IF = 0;
    Idle();
}