/* 
 * File:   Lab05.c
 * Author: pablo
 *
 * Created on 15 de agosto de 2021, 21:16
 */

#include <stdio.h>
#include <stdlib.h>

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
#include "ADC.h"
//#include "EUSART.h"
//#include "configuraciones_pic.h"
//*******************************definiciones***********************************
#define _XTAL_FREQ 8000000 

//*********************************Variables************************************
uint8_t contador;
char contador_string[10];
char unidad, decena, centena;
char concatenado;
int full;
 //********************************Prototipos***********************************
void setup (void);
void mensaje (void);
void putch(char data);
 //********************************Interrupciones*******************************
 void __interrupt() isr(void){  
//--------------------------------interrupcion PORTB----------------------------
   if (INTCONbits.RBIF == 1){ //se revisa si hay interrupción
       
       if(PORTBbits.RB0 == 1){ //se incrementa variable si se toca botón 1
           contador++;
           contador = contador & 0x0f; //solo deja pasar de 0 a 255
   }
       if(PORTBbits.RB1 == 1){ //se decrementa variable si se toca botón 2
           contador--;
           contador = contador & 0x0f; //solo deja pasar de 0 a 255
       }
       INTCONbits.RBIF = 0; //limpiar bandera para reiniciar interrupción
   }
}

//*********************************loop principal*******************************
 void main (void){
     setup();  
     while (1){ 
         mensaje();
         PORTA = full;
         convert (contador_string, contador, 2);
     } 
     return;
}
 //*******************************funciones**************************************
void mensaje (void){
    __delay_ms(300);
    printf("\r valor del contador: \r");
    __delay_ms(300);
    printf(contador_string);       //se manda el valor del contador
    printf("\r---------------\r");
    
    printf("Ingresar Centena: Rango(0-2)\r"); //le pide al usuario ingresar algo
      chistosito1:  
       while(RCIF == 0);
        centena = RCREG -48;  

       while(RCREG > '2'){  //este es una defensa por si alguien se pasa de 
                            //chistoso y mete un dato que no es
           goto chistosito1;
       }
    
    printf("Ingresar Decenas: \r");
      chistosito2:
        while(RCIF == 0); 
         decena = RCREG -48; 

        if(centena == 2){
           while(RCREG > '5'){
               goto chistosito2;    //misma funcion que chistosito 1
           }
       }

    printf("Ingresar Unidades: \r");
      chistosito3:
       while(RCIF == 0); 
        unidad = RCREG - 48;

       if(centena == 2 && decena == 5){
           while(RCREG > '5'){
               goto chistosito3;    //misma función que chistosito 1
           }
       }
      concatenado = concatenar(centena, decena); //se concatenan primeros datos
      full = concatenar(concatenado, unidad); //se termina de concatenar 
      __delay_ms(250);
    printf("El numero elegido es: %d", full); //se manda valor de concatenado final

   return;
}
 
int concatenar(int a, int b){
    char s1[20];    //variables para cadena de caracteres
    char s2[20];
    
    //Aquí se convierten los integers a strings
    sprintf(s1, "%d", a);
    sprintf(s2, "%d", b);
    
    //concatenar los strings
    strcat(s1, s2);
    
    //convertir los strings concatenados a integers
    int c = atoi(s1);
    
    //regresa el valor numerico formado
    return c;
}
void putch(char dato){ 
    while(TXIF == 0);
    TXREG = dato; //lo que se escribe se manda al pic para que lo procese 
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
    OSCCONbits.IRCF2 = 1;       //Reloj interno de 8MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS   = 1;
     
    //-----------------------------conf UART------------------------------------
    //Configuración del RX y TX
    TXSTAbits.SYNC = 0;     //modo sincrono desabilitado
    TXSTAbits.BRGH = 1;     //Bound rate High
    BAUDCTLbits.BRG16 = 1;  //16 bits para generar fire brake
    
    SPBRG = 207;    //SPBRG con 0.16% de error
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1; //Puertos seriales 
    RCSTAbits.RX9 = 0;  //No se van a recibir 9 bits
    RCSTAbits.CREN = 1; //habilitar recepción
    TXSTAbits.TXEN = 1; //habilitar transmisión

     
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

