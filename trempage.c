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

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "system.h"          /* variables/params used by system.c */
#include "trempage.h"
#include "lcd4bit.h"
#include "user.h"

#include <stdio.h>
#include <libpic30.h>


// RAM
Trempage trempage;

// ROM
Trempage __attribute__((space(eedata))) trempageHARD;



/**
 * Gestion de la partie affichage de l'écran LCD
 */
void afficherAccueil() {

    char affichage[20];
    uint16_t var, var1;

    // Affichage du menu config
    if ( trempage.etat != 1 ) {

        clrLcd();
        setStrAt(0, 0, "Nombre de cycles : ");
        sprintf(affichage, "%2d", trempage.cyclesMax);
        setStrAt(6, 1, affichage);
        setBlinkOn();
    }
    // Affichage de l'état du cycle en cours
    else if ( trempage.etat == 1 ) {

        clrLcd();
        sprintf(affichage, "  Cycle %2d/%2d  ", trempage.cycleActuel + 1, trempage.cyclesMax);
        setStrAt(0, 0, affichage);
        var = trempage.cycles[trempage.cycleActuel] * 10;
        var1 = getCycleMinutes();
        sprintf(affichage, " %2dh%2dm /%2dh%2dm  ",
                var1 / 60, var1 % 60, var / 60, var % 60);
        setStrAt(0, 1, affichage);
        setBlinkOff();
    }

    // Fin du cycle de trempage
    if ( trempage.etat == 2 ) {
        setLedFin();
        clrLcd();
        setStrAt(0, 0, "  Fin trempage  ");
    }
}

/**
 * Routine de régulation
 */
void regulation() {

    // Régulation active uniquement en mode état = 1 (ou mode auto)
    if ( getEtat() == 1 ) {

        // Changement de cycle
        if ( getCycleMinutes() >= (getCycleN(getCycleActuel()) * 10) ) {

            setCycleActuel(getCycleActuel() + 1);
            setCycleMinutes(0);

            if ( getCycleActuel() >= getCyclesMax() ) {

                setEtat(2);
                afficherAccueil();
                setCycleMinutes(0);
                setCycleActuel(0);

                remplir(0);
                vider(0);

                setEEPData();

                return;
            }

            // Remplissage
            if ( (getCycleActuel() % 2) == 0 ) {
                remplir(1);
            }                
            // Vidange
            else {
                vider(1);
            }

            setEEPData();
        }

        afficherAccueil();
        
        // Enregisrement régulier des données en cas de coupure
        // % 5 => point de sauvegarde toutes les 5 minutes
        if ( (getCycleMinutes() % 5) == 0 ) {

            int16_t adress = 0xFE26;
            adress += trempage.cycleMinutesIndex * 2;
            writeEEP(adress, getCycleMinutes());
        }

    } else {

        remplir(0);
        vider(0);
    }
}

/**
 * Reprise du cycle en cours (utilisé en cas de coupure)
 */
void repriseCycle() {

    // Remplissage
    if ( (getCycleActuel() % 2) == 0 ) {
        remplir(1);
        setLedEnCours();
    }
    // Vidange
    else {
        vider(1);
        setLedEnCours();
    }

    afficherAccueil();
}


/**
 * RAZ des données EEPROM (reset materiel)
 */
void resetEEPData() {

    trempage.cyclesMax = 0;
    trempage.cycleActuel = 0;
    trempage.cycleMinutesIndex = 0;
    trempage.etat = 0;

    // Inutile vu la façon d'écrire les données
    uint8_t i;
    for( i = 0; i < NB_MAX_CYCLES; i++) {
        trempage.cycles[i] = 0;
        trempage.cycleMinutes[i] = 0;
    }
    
    setEEPData();
}

/**
 * Enregistre les données critiques dans l'eeprom
 */
void setEEPData() {

    uint8_t i = 0;
    uint16_t pointer = 0xFE00;

    writeEEP(pointer, (trempage.cyclesMax + (trempage.cycleActuel << 8)));
    
    pointer += 2;
    writeEEP(pointer, trempage.cycleMinutesIndex);
    
    pointer += 2;
    writeEEP(pointer, trempage.etat);

    for ( i = 0; i < NB_MAX_CYCLES; i += 2 ) {
        
        pointer += 2;
        writeEEP(pointer,(trempage.cycles[i] + (trempage.cycles[i + 1] << 8)));
    }
    
    for ( i = 0; i < NB_MAX_CYCLES; i++ ) {
        
        pointer += 2;
        writeEEP(pointer,trempage.cycleMinutes[i]);
    }
}

/**
 * Récupère tous les paramètres dans l'EEPROM au boot/reset et les charge
 * dans la RAM
 */
void retrieveEEPROMdata() {

    uint8_t i;
    uint16_t data, address = 0xFE00;

    data = readEEP(address);
    
    trempage.cyclesMax = data;
    trempage.cycleActuel = data >> 8;

    address += 2;
    data = readEEP(address);
    trempage.cycleMinutesIndex = data;

    address += 2;
    data = readEEP(address);
    trempage.etat = data;

    for ( i = 0; i < NB_MAX_CYCLES; i += 2 ) {

        address += 2;
        data = readEEP(address);
        trempage.cycles[i] = data;
        trempage.cycles[i+1] = data >> 8;
    }
    
    for( i = 0; i < NB_MAX_CYCLES; i++) {
        
        address += 2;
        data = readEEP(address);
        trempage.cycleMinutes[i] = data;
    }
}

/**
 * Routine de remplissage cuve
 * 
 * @param etat
 */
void remplir(uint8_t etat) {

    if ( etat == 0 ) {

        EV_REMPLIR = 0;
    } else {

        EV_VIDER = 0;
        EV_REMPLIR = 1;
    }
}

/**
 * Routine de vidange cuve
 * 
 * @param etat
 */
void vider(uint8_t etat) {

    if ( etat == 0 ) {

        EV_VIDER = 0;
    } else {

        EV_REMPLIR = 0;
        EV_VIDER = 1;
    }


}

void setCyclesMax(uint8_t cyclesMax) {
    trempage.cyclesMax = cyclesMax;
}

uint8_t getCyclesMax(void) {
    return trempage.cyclesMax;
}

uint8_t getCycleN(uint8_t n) {
    return trempage.cycles[n];
}

void setCycleN(uint8_t n, uint8_t value) {
    trempage.cycles[n] = value;
}

uint8_t getEtat() {
    return trempage.etat;
}

void setEtat(uint8_t etat) {
    trempage.etat = etat;
}

uint8_t getCycleActuel() {
    return trempage.cycleActuel;
}

void setCycleActuel(uint8_t cycleActuel) {
    trempage.cycleActuel = cycleActuel;
}

uint16_t getCycleMinutes() {
    return trempage.cycleMinutes[trempage.cycleMinutesIndex];
}

void setCycleMinutes(uint16_t cycleMinutes) {
    trempage.cycleMinutes[trempage.cycleMinutesIndex] = cycleMinutes;
}

void setCycleMinutesIndex(uint16_t index) {
    trempage.cycleMinutesIndex = index;
}

uint16_t getCycleMinutesindex() {
    return trempage.cycleMinutesIndex;
}

void setLedEnCours() {
    LED_EN_COURS = 1;
    LED_FIN = 0;
    LED_PAUSE = 0;
}

void setLedPause() {
    LED_EN_COURS = 0;
    LED_FIN = 0;
    LED_PAUSE = 1;
}

void setLedFin() {
    LED_EN_COURS = 0;
    LED_FIN = 1;
    LED_PAUSE = 0;
}
