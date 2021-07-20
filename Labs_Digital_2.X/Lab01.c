/* 
 * File:   Lab01.c
 * Author: pablo
 *
 * Created on 15 de julio de 2021, 12:07
 */

#include <stdio.h>
#include <stdlib.h>
#include "configuraciones_pic.h"
#include <stdint.h>
#include <xc.h>// </editor-fold>

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

#define _XTAL_FREQ 800000
//------------------------------tabla-------------------------------------------
uint8_t tabla [16] ={0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X67,
                      0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};
//------------------------------variables---------------------------------------
uint8_t flags;
uint8_t down_nibbles;
uint8_t upper_nibbles;
uint8_t ADC;
//------------------------------prototipos--------------------------------------
void setup (void);
//------------------------------interrupciones----------------------------------
void __interrupt() interrupiones(void){  
//-------------------------------ADC
    if (PIR1bits.ADIF == 1){ //revisa si hay interrupción de ADC
        if (ADCON0bits.CHS == 0){ //revisar si hay coversion en canal 0
            ADC_convert(ADRESH);   
        }
        PIR1bits.ADIF = 0; //se limpia la vandera de interrupcion
    }
//-------------------------------PORTB
    if (RBIF == 1){
        if(RB0 == 1){
            PORTD++;
        }
        if(RB1 == 1){
            PORTD--;
        }
    INTCONbits.RBIF = 0; 
    }
//-------------------------------Timer0  
    if (T0IF == 1){
 
        PORTAbits.RA1 = 0;
        PORTAbits.RA2 = 0;          //se limpian los puertos a utilizar
        
        INTCONbits.T0IF  = 0;       //Reinicio del timer 0
        TMR0 = 255;  
        
        if (flags == 1) {           //Por medio de banderas verificamos que 
           PORTAbits.RA1 = 0;       //display es el que toca encender
           PORTAbits.RA2 = 1;       //al terminar el if cambiamos de bandera
           display(down_nibbles);
           flags = 0; 
        }
        
        else {
           PORTAbits.RA2 = 0;
           PORTAbits.RA1 = 1;
           display(upper_nibbles);
           flags = 1;  
        }          
    }    
}

//----------------------------------loop princiapl------------------------------
void main(void){
    setup();
    ADCON0bits.GO = 1; //se inicia la conversion del ADC
    
    while(1){
           if (ADCON0bits.GO == 0){        //Cuando termine la conversion 
            __delay_us(200);            //
            ADCON0bits.GO = 1;         
        }                               
        
        nibbles(ADC);
        
        if (ADC > PORTD) {
            PORTAbits.RA3 = 1;
        }
        
        else {
            PORTAbits.RA3 = 0;
        }        
    }
    return;
}

//----------------------------------Configuraciones-----------------------------
void setup (void){
    ANSEL = 0b00000001;
    ANSELH = 0;
    
    TRISA = 0b00000001;
    TRISB = 0b00000011;
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    
    PORTA = 0b00000000;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000000;
    
   //Configuracion Interupciones
    INTCONbits.GIE   = 1;     //Interupciones on change y del TMR0
    INTCONbits.PEIE  = 1;
    INTCONbits.T0IE  = 1;
    INTCONbits.RBIE  = 1;
    PIE1bits.ADIE    = 1;
    
    INTCONbits.T0IF  = 0;
    INTCONbits.RBIF  = 0;
    PIR1bits.ADIF    = 0;
    
    //------reloj oscilador
    conf_osc(7);
    
    //------timer0
    conf_tmr0(7); //la velocidad de interrupcion es cada 0.00012800s
    TMR0 = 255;
    
    //------Configuracion puerto B
    IOCBbits.IOCB0   = 1;       //RB0 y RB1  interrupt on change activado
    IOCBbits.IOCB1   = 1;
    
    //------configuracion ADC
    ADCON1bits.ADFM  = 0;    //Justificado a la izquierda
    conf_ADC(2);
    ADCON0bits.CHS   = 0;    //Chanel 0
    __delay_us(200);
    ADCON0bits.ADON  = 1;    //Encendemos el ADC
    __delay_us(200);
    
    
    //Valor inicial del multiplexado
    flags = 0b00000000;       //Valor inicial de la bandera para multiplexeado
    PORTD = 0b00000000;       //Contador binario    
    
}