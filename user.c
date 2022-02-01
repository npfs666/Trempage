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

#include "user.h"            /* variables/params used by user.c */
#include "lcd4bit.h"
#include "trempage.h"
#include "system.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/



void InitApp(void)
{
    /* Setup analog functionality and port direction */
    ANSELA = 0;
    ANSELB = 0;
    ANSA = 0;
    ANSB = 0;

    TRISA = 0;
    PORTA = 0;
    TRISAbits.TRISA4 = 1;   // Quartz 32 768Hz
    TRISAbits.TRISA7 = 1;   // Rotenc
    
    TRISB = 0;
    PORTB = 0;
    TRISBbits.TRISB4 = 1;   // Quartz 32 768Hz
    TRISBbits.TRISB8 = 1;   // Rotenc
    TRISBbits.TRISB9 = 1;   // Bouton defil
    TRISBbits.TRISB12 = 1;  // Bouton Reset EEP
    TRISBbits.TRISB13 = 1;  // Bouton Auto/Man
    TRISBbits.TRISB14 = 1;  // Bouton Remplissage
    TRISBbits.TRISB15 = 1;  // Bouton Vidange
    

    /* Initialize peripherals */    
    initLCD();
    initRTCC();
    startRTC();
    initCOSInterrupt();
    initICRotaryEncoder();
}



void setRTCWREN(void) {

    //Débloquage du bit RTCWREN pour autoriser la modification du registre RTCC
    asm volatile ("push w7");
    asm volatile ("push w8");
    asm volatile ("disi #5");
    asm volatile ("mov #0x55, w7");
    asm volatile ("mov w7, _NVMKEY");
    asm volatile ("mov #0xAA, w8");
    asm volatile ("mov w8, _NVMKEY");
    asm volatile ("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile ("pop w8");
    asm volatile ("pop w7");
}



void initRTCC(void) {

    setRTCWREN();

    // Configuration du RTCC
    RTCPWCbits.RTCCLK = 0b00;   // external secondary oscillator  (SOSC)
    ALCFGRPTbits.CHIME = 1;     // Décompte à l'infini
    ALCFGRPTbits.AMASK = 0b0011;// Every minute (0011)

    // Activation de l'IT du RTCC
    IEC3bits.RTCIE = 1;         //Real-Time Clock and Calendar Interrupt Enable bit
    IPC15bits.RTCIP = 0b010;    // priorité 2
    ALCFGRPTbits.ALRMEN = 1;    // enable alarm
    
    _RTCIF = 0;
}



void startRTC(void) {
    setRTCWREN();
    RCFGCALbits.RTCEN = 1;
}



void stopRTC(void) {
    setRTCWREN();
    RCFGCALbits.RTCEN = 0;
}



/**
 * Initialisation des IT "boutons"
 */
void initCOSInterrupt(void) {
    
    // Les pull-up sont intégrées à la carte pas besoin de les mettre
    
    CNEN1bits.CN14IE = 1;    // Bouton reset EEP
    CNEN2bits.CN21IE = 1;    // Bouton defil
    CNEN1bits.CN13IE = 1;    // Bouton Auto/Man
    CNEN1bits.CN12IE = 1;    // Bouton Remplissage
    CNEN1bits.CN11IE = 1;    // Bouton Vidange

    // Activation des IT
    IPC4bits.CNIP = 0b001;  // Priorité 1
    
    IEC1bits.CNIE = 1;      // Activation des IT "changement d'état"
    
    _CNIF = 0;
}



void initICRotaryEncoder(void) {
    
    uint16_t value;
    
    // On lis le buffer tant qu'il n'est pas clear
    while(IC1CON1bits.ICBNE) value = IC1BUF;
    
    IC1CON2bits.SYNCSEL = 0b0;      // Pas de trigger, free counting
    IC1CON1bits.IC1TSEL = 0b111;    // fosc/2
    IC1CON1bits.ICI = 0b00;         // Capture every event
    IC1CON2bits.ICTRIG = 0b0;       // Syncsel selects the trigger
    IC1CON1bits.ICM = 0b010;        // Capture every falling edge
    
    // Activation des IT sur le IC1
    IEC0bits.IC1IE = 1;
    IPC9bits.IC3IP = 0b001;         // Priorité 1
    
    _IC1IF = 0;
}