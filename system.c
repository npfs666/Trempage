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

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c                                           */
/*                                                                            */
/******************************************************************************/



void ConfigureOscillator(void) {

    CLKDIVbits.RCDIV = 0b010;   // Postscaler 1:4 -> 2MHz
}



/**
 * Lis la valeur stockée à l'adresse donnée
 * @param adress Adresse mémoire EEPROM
 * @return la valeur lue, 16bits
 */
int16_t readEEP(const uint16_t adress) {

    uint16_t addr = adress;

    TBLPAG = 0x7F; // Initialize EE Data page pointer

    return __builtin_tblrdl(addr);
}



/**
 * Ecris 16bits dans l'eeprom (erase + ecriture ensuite)
 *
 * @param address adresse du dword
 * @param data  données à écrire
 */
void writeEEP(uint16_t address, int16_t data) {

    TBLPAG = 0x7F; // Initialize EE Data page pointer

    NVMCON = 0x4058; // Set up NVMCON to erase one word of data EEPROM
    
    __builtin_tblwtl(address, 0xffff);
    asm volatile ("disi #5");
    __builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
    while (NVMCONbits.WR); // wait for last write to complete

    
    // Set up NVMCON to erase one word of data EEPROM
    NVMCON = 0x4004;

    __builtin_tblwtl(address, data); // Write EEPROM data to write latch
    asm volatile ("disi #5"); // Disable Interrupts For 5 Instructions
    __builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
    while (NVMCONbits.WR); // Optional: Poll WR bit to wait for
}


/**
 * Lis en mémoire RTC le jour du mois
 * 
 * @return jour[1;31]
 */
uint16_t getRTCCDay() {

    uint16_t rtcval, day;

    RCFGCALbits.RTCPTR=0b10;
    rtcval = RTCVAL;

    //*mois = ((rtcval&0x1000)>>12)*10 + ((rtcval&0x0F00)>>8);
    day = ((rtcval&0x0030)>>4)*10 + (rtcval&0x000F);
    
    return day;
}