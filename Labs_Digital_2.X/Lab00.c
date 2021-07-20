/* 
 * File:   Lab00.c
 * Author: pablo
 *
 * Created on 12 de julio de 2021, 19:31
 */

#include <stdio.h>
#include <stdlib.h>

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements
// CONFIG1

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>// </editor-fold>

//--------------------------------valores definidos-----------------------------
#define N_timer0 250
//----------------------------------tabla---------------------------------------
//los numeros incrementan de 0 hasta 9 
char tabla [10] = {0B00111111, 0B00000110, 0B01011011, 0B01001111, 0B01100110,
0B01101101, 0B01111101, 0B00000111, 0B01111111, 0B01100111};

//----------------------------------variables-----------------------------------
int semaforo;
int flags;
//---------------------------------prototipos-----------------------------------
void setup (void);
//-------------------------------interrupciones---------------------------------
void __interrupt()interrupcion(void){
 if (T0IF == 1) {
    semaforo--; 
    flags = 0;
    
    if (semaforo == 3){
       PORTAbits.RA4 = 1;
    }
    else if (semaforo == 2){
        PORTAbits.RA5 = 1;
    } 
    else if (semaforo == 1){
        PORTAbits.RA6 = 1; 
    }
    else if (semaforo == 0){
        PORTAbits.RA5 = 0;
        PORTAbits.RA4 = 0;
        PORTAbits.RA6 = 1; 
    }
 }
 
}
void main (){
 setup(); 
 while(1){
     if(PORTBbits.RB0 == 1){
         semaforo = 3;
         PORTB = tabla(semaforo);   
     }
 }
}