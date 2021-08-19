/* 
 * File:   Lab05.c
 * Author: pablo
 *
 * Created on 15 de agosto de 2021, 21:16
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
#include <string.h> // Concatenar
#include "EUSART.h"
#include "configuraciones_pic.h"
#include "ADC.h"
//*******************************definiciones***********************************
#define _XTAL_FREQ 8000000 

//*********************************Variables************************************
uint8_t contador;
char contador_string[10];
char ingreso, pos, total;
char centena, decena, unidad;
char entrante [2];
 //********************************Prototipos***********************************
void setup (void);
char centenas (int dato);
char decenas (int dato);
char unidades (int dato);
 //********************************Interrupciones*******************************
 void __interrupt() isr(void){  
//--------------------------------interrupcion PORTB----------------------------
   if (INTCONbits.RBIF == 1){ //se revisa si hay interrupción
       
       if(PORTBbits.RB0 == 1){ //se incrementa variable si se toca botón 1
           contador++;
   }
       if(PORTBbits.RB1 == 1){ //se decrementa variable si se toca botón 2
           contador--;
 
       }
       INTCONbits.RBIF = 0; //limpiar bandera para reiniciar interrupción
   }
}

//*********************************loop principal*******************************
 void main (void){
    setup();  
    while (1){
        centena = centenas(contador);
        decena = decenas(contador);
        unidad = unidades(contador);
        centena += 48;
        decena += 48;
        unidad += 48;
        if (PIR1bits.RCIF == 1){ //compruebo si se introdujo un dato
            ingreso = USART_Recieve();
            
            if(ingreso == 's'){
                USART_Transmit(centena);
                USART_Transmit(decena);
                USART_Transmit(unidad);
            }
            
            if(ingreso > 47 && ingreso < 58){
                entrante[pos] = ingreso;
                pos++;
                //PORTD++;
                if (pos > 2){
                    pos = 0;
                    total = (entrante[0] - 48) * 100;
                    total +=(entrante[1] - 48) *10;
                    total +=(entrante[2] - 48);
                    PORTA = total;
                    //PORTD++;
                }
            }
       }
        ingreso = 0;
    }
    return;
 }
 //*******************************funciones**************************************
char centenas (int dato){
    char out = dato / 100;
    return out;
}

char decenas (int dato){
    char out;
    out = (dato % 100) / 10;
    return out;
}

char unidades (int dato){
    char out;
    out = (dato % 100) % 10;
    return out;
}
 
//int concatenar(int a, int b){
//    char s1[20];    //variables para cadena de caracteres
//    char s2[20];
//    
//    //Aquí se convierten los integers a strings
//    sprintf(s1, "%d", a);
//    sprintf(s2, "%d", b);
//    
//    //concatenar los strings
//    strcat(s1, s2);
//    
//    //convertir los strings concatenados a integers
//    int c = atoi(s1);
//    
//    //regresa el valor numerico formado
//    return c;
//}

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
      conf_osc(7); //Reloj interno de 8MHz

     
    //-----------------------------conf UART------------------------------------
    void init_USART (void);
 //-------------------------configuración de interrpciones----------------------
    INTCONbits.GIE = 1; 
    INTCONbits.PEIE = 1;
    INTCONbits.RBIE = 1; //Interrupcion de puerto B activada
    
    INTCONbits.RBIF = 0; //bajar la bandera de interrupcion en PORTB    
    //Configuracion Puerto B
    IOCBbits.IOCB0   = 1;       //Activamos en RB0 y RB1 la interupcion on change
    IOCBbits.IOCB1   = 1;
    
//    //Interrupciones de transmisión recepción activadas
//    PIE1bits.RCIE = 1;
//    PIE1bits.TXIE = 1;
//    //banderas de transmisión y recepción se bajan
//    PIR1bits.TXIF = 0; 
//    PIR1bits.RCIF = 0; 
 }