/* 
 * File:   Lab03_master.c
 * Author: pablo
 *
 * Created on 1 de agosto de 2021, 22:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include <string.h> // Concatenar
#include "configuraciones_pic.h"
#include "SPI.h"
#include "ADC.h"

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

//-----------------------------valores definidos--------------------------------
#define _XTAL_FREQ 8000000

//------------------------------variables---------------------------------------
volatile uint8_t pot1;
volatile uint8_t pot2;
char adc0[10];
char adc1[10];
float conv0 = 0;
float conv1 = 0;
char unidad, decena, centena;
char concatenado;
int full;

//------------------------------prototipos--------------------------------------
void setup (void);
void mensaje (void);
void putch(char data);
//----------------------------------loop princiapl------------------------------
void main(void){
    setup();
 
    while(1){
        mensaje();
        PORTCbits.RC2 = 0;  //seleccion del esclavo
        __delay_ms(1);
        
        spiWrite(1); //le digo al asclavo que quiero valot de pot1
        pot1 = spiRead();
        __delay_ms(1);
        PORTCbits.RC2 = 1; //se deja de usar slave1
        
        PORTCbits.RC2 = 0; //se selecciona el slave 1 de nuevo
        __delay_ms(1);
        
        spiWrite(2); //se avisa al esclavo que quiero el valor de pot2
        pot2 = spiRead(); //guardo el valor 
        
        __delay_ms(1);
        PORTCbits.RC2 = 1; //desleccionar esclavo 1    }
    
        conv0 = 0;
        conv1 = 0;
        
        conv0 = (pot1 / (float) 255)*5;  
        conv1 = (pot2 / (float) 255)*5; 
        
        ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.
        ADC_convert(adc1, conv1, 2);
        PORTB = full;
    }
    return;
}
//*******************************funciones**************************************
void mensaje (void){
    __delay_ms(300);
    printf("\r voltaje 1: \r");
    __delay_ms(300);
    printf(adc0);       //se manda el valor de la conversión de ADC canal 0
    printf("\r---------------\r");
    
    __delay_ms(300);
    printf("\r voltaje 2: \r");
    __delay_ms(300);
    printf(adc1);       //se manda el valor de la conversión de ADC canal 1
    __delay_ms(300);
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
 
void putch(char dato){ 
    while(TXIF == 0);
    TXREG = dato; //lo que se escribe se manda al pic para que lo procese 
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
//----------------------------------Configuraciones-----------------------------
void setup (void){
    ANSEL = 0x00;
    ANSELH = 0;
    
    TRISB = 0x00;
    TRISD = 0x00;
    
    //------conf SPI
    PORTB = 0x00;
    PORTD = 0x00;
    
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    
    //------reloj oscilador
    conf_osc(7); //oscilador de 8MHz
    
    //------------conf UART
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
    
    //Configuracion de interrupciones
    INTCONbits.GIE = 1; //globales
    INTCONbits.PEIE = 1;
    //Interrupciones de transmisión recepción activadas
    PIE1bits.RCIE = 1;
    PIE1bits.TXIE = 1;
    //banderas de transmisión y recepción se bajan
    PIR1bits.TXIF = 0; 
    PIR1bits.RCIF = 0; 
}
