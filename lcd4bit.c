/*
 * File:   lcd.c
 * Author: Brendan
 *
 * Created on 31 janvier 2017
 */

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

#include <stdint.h>

#include "lcd4bit.h"
#include "system.h"
#include <libpic30.h>



/**
 * Initialisation du LCD
 *
 * Communication du registre à décalage -> LCD : 4 bits
 * Cursor : off
 * Blink : off
 */
void initLCD(void) {

    __delay_ms(100);
    
    // Si c'est le premier boot on initialise le LCD
    //if( (_EXTR != 1) && (_WDTO != 1) ) {
        
        RS = 0;
        
        // Init écran chintok (HD44780)
        /*sendNibble(0x02);
        __delay_us(5);
        sendNibble(0x02);
        __delay_us(5);
        sendNibble(0x0C);
        __delay_ms(1);*/
        
        // Init écran MIDAS (SPLC780D)
        sendNibble(0x03);
        __delay_ms(5);
        sendNibble(0x03);
        __delay_ms(1);
        sendNibble(0x03);
        __delay_ms(1);
        sendNibble(0x02);
        __delay_ms(1);

        
        setInstruction(0x0C); // set display on cursor oFF blink off to on the cursor E to and on the blink F
        setInstruction(0x06); // cursor shift direction
    //}

    //_EXTR = 0;
    //_WDTO = 0;
    
    clrLcd();
}



/**
 * Méthode interne, privée
 *
 * Envoie un demi octet sur le LCD 
 *
 * @param d DATA
 */
void sendNibble( uint8_t d ) {

    if( d & 0x01 )
        D4 = 1;
    else
        D4 = 0;
    
    if( d & 0x02 )
        D5 = 1;
    else
        D5 = 0;
    
    if( d & 0x04 )
        D6 = 1;
    else
        D6 = 0;
    
    if( d & 0x08 )
        D7 = 1;
    else
        D7 = 0;
    
    
    /*D4 = d & 0x01;
    D5 = d | 0x02;
    D6 = d | 0x04;
    D7 = d | 0x08;*/

    __delay_us(1);
    
    E = 1;              // Front montant sur E

    __delay_us(1);

    E = 0;
}


/**
 * Envoie une commande au LCD
 *
 * @param instr
 */
void setInstruction(uint8_t instr)
{
    RS = 0; //because sending command

    uint8_t tmp = instr;
    sendNibble(instr>>4);
    sendNibble(tmp & 0x0F);

    __delay_us(50);	// Temps de traitement de l'afficheur
}



/**
 * RAZ de l'écran du LCD
 */
void clrLcd(void) {

    setInstruction(0x01);
    __delay_us(1600);    // 1.6mS
}



/**
 * Méthode interne, privée
 *
 * @param byte
 */
void setData(int8_t byte) {

    RS = 1; //because sending data

    sendNibble(byte >> 4);
    sendNibble(byte & 0xF);

    __delay_us(50);     // Temps de traitement de l'afficheur
}



/**
 * Envoie une chaine de caractères à la position actuelle du curseur
 *
 * @param str
 */
void setStr(char *str) {

    uint8_t i = 0;

    while(str[i] != 0) {

        //if(i == NB_CHAR_LIGNE) envoiCommande(0xD4); // saut ligne
        setData(str[i]);
        i++;
    }
}



/**
 * Déplace le curseur aux coordonnées précisées.
 *
 * @param col   X
 * @param row   Y
 *
 * @return
 *  0 : ok
 *  1 : index out of array
 */
uint8_t setCursorAt(uint8_t col, uint8_t row) {

    // Index out of array
    if( (col > NB_COL) || ( row > NB_ROW ) ) return 1;

    // Si ligne impair
    if( row % 2 ) {
        setInstruction(0xC0+col+0x14*(row/2));
    } else {
        setInstruction(0x80+col+0x14*(row/2));
    }

    return 0;
}



/**
 * Affiche une chaine de caractères aux coordonnées précisées.
 *
 * @param col
 * @param row
 * @param str
 * @return
 */
uint8_t setStrAt(uint8_t col, uint8_t row, char* str) {

    uint8_t r = setCursorAt(col, row);

    if( !r ) {
        setStr(str);
        return 0;
    }
    else
        return 1;
}



/**
 * Active le clignotement du curseur
 */
void setBlinkOn(void) {
    setInstruction(0x0D);
}



/**
 * Désactive le clignotement du curseur
 */
void setBlinkOff(void) {
    setInstruction(0x0C);
}