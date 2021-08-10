/* 
 * File:   Lab04_Slave2.c
 * Author: pablo
 *
 * Created on 9 de agosto de 2021, 12:31
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

#define _XTAL_FREQ 8000000 

//*********************************Variables************************************
uint8_t z;
uint8_t lectura;
uint8_t contador;
 //********************************Prototipos***********************************
 void setup(void);
 //********************************Interrupciones*******************************
 void __interrupt() isr(void){
 //--------------------------------interrupion I2C------------------------------    
   if(PIR1bits.SSPIF == 1){ 
        
        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
            
        }
//----------------------------hacer al esclavo leer-----------------------------
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            lectura = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
//------------------------------rl esclavo manda datos--------------------------         
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = contador; //manda el la conversion del ADC
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }

//--------------------------------interrupcion PORTB----------------------------
   if (INTCONbits.RBIF == 1){
       if(PORTBbits.RB0 == 1){
           contador++;
           contador = contador & 0x0f;
   }
       if(PORTBbits.RB1 == 1){
           contador--;
           contador = contador & 0x0f;
       }
       INTCONbits.RBIF = 0; //limpiar bandera para reiniciar interrupción
   }
}

//*********************************loop principal*******************************
 void main (void){
     setup();  
     while (1){ 
         PORTA = contador;
     } 
     return;
}
 //********************************configuraciones******************************
 void setup(void){
     ANSEL = 0X00;
     ANSELH = 0X00;
 //-----------------------------SELECCIONAR INPUTS Y OUTPUTS--------------------    
     TRISA = 0X00;
     TRISB = 0x03;
 //------------------------------LIMPIAR PUERTOS--------------------------------    
     PORTA = 0X00;
     PORTB = 0x00;
//------------------------------configuracion del oscilador---------------------
     //conf_osc(7); 
    //Configuracion del Oscilador
    OSCCONbits.IRCF2 = 1;       //Reloj interno de 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS   = 1;
    //--------------------------configuracion de I2C----------------------------    
    I2C_Slave_Init(0X20); //inicializar comunicación I2C del esclavo y se le da
    //dirección al esclavo, en este caso 0x10
 //-------------------------configuración de interrpciones----------------------
    INTCONbits.GIE = 1; //Interrupcion de puerto B activada
    INTCONbits.PEIE = 1;
    INTCONbits.RBIE = 1;
    
    INTCONbits.RBIF = 0; //bajar la bandera de interrupcion en PORTB    
    //Configuracion Puerto B
    IOCBbits.IOCB0   = 1;       //Activamos en RB0 y RB1 la interupcion on change
    IOCBbits.IOCB1   = 1;
 }
 