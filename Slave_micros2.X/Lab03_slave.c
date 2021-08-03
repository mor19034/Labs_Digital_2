/* 
 * File:   Lab03_slave.c
 * Author: pablo
 *
 * Created on 1 de agosto de 2021, 7:47
 */

//----------------------------librerias a utilizar------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ADC.h"
#include "SPI_slave.h"

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
//---------------------------------Variables------------------------------------
uint8_t canal_flag = 0;
volatile uint8_t var_adc0 = 0;
volatile uint8_t var_adc1 = 0;
char adc0[10];
char adc1[10];
float conv0 = 0;
float conv1 = 0;

//------------------------------prototipos--------------------------------------
void setup (void);
void mensaje (void);
//--------------------------------interrupciones--------------------------------
void __interrupt() isr(void){
   if(SSPIF == 1){
       uint8_t write;
       write = spiRead();
       
       switch(write) {
           case 1:
               spiWrite(var_adc0);
               break;
           
           case 2:
               spiWrite(var_adc1);
               break;      
       }
       
       PIR1bits.SSPIF = 0;
    }
}
//----------------------------------loop princiapl------------------------------
void main(void){
    setup();
    canal_ADC(0); //Se habilita canal RA0 para ADC
    canal_ADC(3); //Se habilita canal RA3 para ADC
    Select_ch(0); //la conversion ADC empieza en canal 0 (RA0)

    while(1){        
        if (PIR1bits.ADIF == 1) { //se revisa si hubo interrupcion del ADC
            if (canal_flag == 0) {
                var_adc0 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(0);
                canal_flag++;
            } else {
                var_adc1 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(3);
                canal_flag--;
            }
            PIR1bits.ADIF = 0;
        }
   
    
//     conv0 = 0;//se reinicia cada vez que se inicia el proceso de enviar datos
//     conv1 = 0;
//        
//        conv0 = (var_adc0 / (float) 255)*5; //Se consigue el porcentaje con 
//        //respecto al valor maximo que un puerto puede tener, despues se 
//        //multiplica por 5 para conocer el voltaje actual del puerto                                          
//        ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.
//        
//        conv1 = (var_adc1 / (float) 255)*5; //misma logica que conv0
//        ADC_convert(adc1, conv1, 2);
        
    }
        
    return;
}
//*******************************funciones**************************************

//----------------------------------Configuraciones-----------------------------
void setup (void){
    ANSEL = 0x09;
    ANSELH = 0;
    
    TRISA = 0x09;
    
    PORTA = 0x00;
    
    TRISAbits.TRISA5 = 1; //slave selected
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //------reloj oscilador
    OSCCONbits.IRCF = 111; //8MHz
    OSCCONbits.SCS = 1; //se utiliza el reloj interno del sistema
    //------configuracion ADC
    conf_ADC(2, 0, 3, 0);    /*Fosc/32 para tads de 4us, sin interrupciones 
                              voltajes VSS y VDD y justificado a la izquierda*/
    
    //Configuracion de interrupciones
    INTCONbits.GIE = 1; //globales
    INTCONbits.PEIE = 1; 
    PIE1bits.SSPIE = 1; //interrupcion de MSSP
    
    PIR1bits.SSPIF = 0; //se baja la bandera de MSSP
}
