/*
 * File:   main.c
 * Author: Adrian
 *
 * Created on March 13, 2024, 8:44 AM
 */

// CONFIG
#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000

enum TURNING_DIRECTION {STRAIGHT, LEFT, RIGHT};

enum TURNING_DIRECTION direction = STRAIGHT;

void signal_right(unsigned int tik) {
    RB6 = tik >= 500 ? 1 : 0;
}

void signal_left(unsigned int tik) {
    RB5 = tik >= 500 ? 1 : 0;
}

void main(void) {
    TRISB = 0x00; // Zustand 0 = Ausgang
    CMCON = 0x07; // Disable comparator
    OPTION_REG = 0b11000010;
    unsigned int tik = 0;
    
    while(1) {
        switch (direction) {
            case RIGHT:
                signal_right(tik);
                break;
            case LEFT:
                signal_left(tik);
                break;
        }
        
        if (tik++ >= 1000) tik = 0;
        
        if ((PORTA & 0x01)) {
            __delay_ms(200);
            PORTB = 0;
            direction = RIGHT;
        }
        else if ((PORTA & 0x02)) {
            __delay_ms(200);
            PORTB = 0;
            direction = STRAIGHT;
        }
        else if ((PORTA & 0x04)) {
            __delay_ms(200);
            PORTB = 0;
            direction = LEFT;
        }
        
        __delay_ms(1);
    }
    return;
}
