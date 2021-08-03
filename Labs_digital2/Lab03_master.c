/* 
 * File:   Lab03_master.c
 * Author: pablo
 *
 * Created on 1 de agosto de 2021, 22:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "configuraciones_pic.h"
#include "SPI.h"

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements
// CONFIG1

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
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
#define _XTAL_FREQ 800000

//------------------------------variables---------------------------------------
uint8_t pot1;
uint8_t pot2;
uint8_t varSC;
//------------------------------prototipos--------------------------------------
void setup (void);
//void mensaje (void);
//----------------------------------loop princiapl------------------------------
void main(void){
    setup();
    
    while(1){
        PORTCbits.RC2 = 0;  //seleccion del esclavo
        __delay_ms(5);
        
        spiWrite(1); //le digo al asclavo que quiero valot de pot1
        pot1 == spiRead();
        
        __delay_ms(1);
        PORTCbits.RC2 = 1; //se deja de usar slave1
        
        PORTCbits.RC2 = 0; //se selecciona el slave 1 de nuevo
        
    }
    return;
}
//*******************************funciones**************************************
//void mensaje (void){
//    __delay_ms(300);
//    printf("\r voltaje 1: \r");
//    __delay_ms(300);
//    printf(adc0);
//    
//    __delay_ms(300);
//    printf("\r voltaje 2: \r");
//    __delay_ms(300);
//    printf(adc1);
//    __delay_ms(300);
//      
//    __delay_ms(300);
//    printf("\r Contador: \r");
//    __delay_ms(300);
//    printf(cont);
//     __delay_ms(300);  
//     
//    if (RCREG == '+'){
//        contador++;
//        RCREG = 0;
//    }
//    else if (RCREG == '-'){
//       contador--;
//       RCREG = 0;
//    }
//    else {
//        NULL;
//    }
//} 
//void putch(char dato){ 
//    while(TXIF == 0);
//    TXREG = dato; //lo que se escribe se manda al pic para que lo procese 
//    return; 
//}
//----------------------------------Configuraciones-----------------------------
void setup (void){
    ANSEL = 0x00;
    ANSELH = 0;

    TRISB = 0x00;
    TRISC = 0b00101000;
    
    PORTA = 0x00;
    PORTB = 0x00;
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


