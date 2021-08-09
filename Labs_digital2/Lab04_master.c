/* 
 * File:   Lab04_master.c
 * Author: pablo
 *
 * Created on 7 de agosto de 2021, 13:28
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//----------------------------librerias a utilizar------------------------------
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2C.h"
#include "ADC.h"
#include "configuraciones_pic.h"

#define _XTAL_FREQ 8000000 

//**********Variables***********

 //**********Prototipos*********
 void setup(void);

 //**********Interrupcion********* 
//********************************loop principl*********************************
void main(void) {
    setup(); 
                
     while (1) {
         I2C_Master_Start();
         I2C_Master_Write(0x51); //va a esclavo con direccion 0x10 y le dice que
                                 //va a leer
         PORTB = I2C_Master_Read(0);
         I2C_Master_Stop();
         __delay_us(200);
        }      
    return;
}
void setup(void){
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    
    PORTA = 0X00;
    PORTB = 0x00;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    
    //Configuracion del Oscilador
    conf_osc(7); //8MHz
}

