/* 
 * File:   Lab03_slave.c
 * Author: pablo
 *
 * Created on 1 de agosto de 2021, 7:47
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
#include "SPI_slave.h"
#include "ADC.h"

#define _XTAL_FREQ 8000000 

//**********Variables***********
uint8_t canal_flag = 0;
uint8_t var_adc0;
uint8_t var_adc1;
 //**********Prototipos*********
 void setup(void);

 //**********Interrupcion********* 
 void __interrupt() isr(void){
   if(SSPIF == 1){
       uint8_t comando;
       comando = spiRead();
       PORTB++;
       switch (comando){
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
void main(void) {
    setup();
    start_adc(3, 0, 0, 0); //Fosc/32, sin interrupcion
                           //de ADC, Ref Vdd y Vcc, justificado a la izquierda
    start_ch(0); //Habilita el pin del Puerto RA0.
    start_ch(1); //Habilita el pin del Puerto RA1.
    Select_ch(0); //Selecciona el canal e inicia la conversion.
    
                
     while (1) {
         if (PIR1bits.ADIF == 1 && ADCON0bits.GO == 0) {
            if (canal_flag == 0) {
                var_adc0 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(0);
                canal_flag++;
                
            } else {
                var_adc1 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(1);
                canal_flag--;
                
                
            }
            __delay_us(200);
            ADCON0bits.GO = 1;
            PIR1bits.ADIF = 0;
        }
                
     }
    return;
}
void setup(void){
    ANSEL = 0x03;
    ANSELH = 0x00;
    TRISAbits.TRISA5 = 1;       // Slave Select
    TRISA = 0x03;
    TRISB = 0x00;
    PORTA = 0X00;
    PORTB = 0x00;
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
    //Configuracion del Oscilador
    config_osc(7); //8MHz
        
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
}