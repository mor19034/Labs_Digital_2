/* 
 * File:   configuraciones_pic.c
 * Author: pablo moreno
 *
 * Created on 15 de julio de 2021, 12:30
 */
#include <stdio.h>
#include <stdlib.h>
#include "configuraciones_pic.h"
#define _XTAL_FREQ 800000
//-----------------------------variables----------------------------------------


//-----------------------------tabla de datos hezadecimales---------------------
uint8_t tabla [16] ={0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X67,
                      0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71};


/*******************************************************************************
 * Funciones para configurar oscilador interno 
 * Parametros: tipos de frecuencia a utilizar 
 ******************************************************************************/
void conf_osc(uint8_t frec){
    switch(frec){                   //31kHz
        case 0: 
            OSCCONbits.IRCF = 000;     
            break;   
        case 1:                     //125kHz
            OSCCONbits.IRCF = 001;     
            break;
        case 2:                     //250kHz
            OSCCONbits.IRCF = 010;     
            break;
        case 3:                     //500kHz
            OSCCONbits.IRCF = 011;     
            break;
        case 4:                     //1MHz
            OSCCONbits.IRCF = 100;     
            break;
        case 5:                     //2MHz
            OSCCONbits.IRCF = 101;     
            break;
        case 7:                     //8MHz
            OSCCONbits.IRCF = 111;     
            break;   
        default:                     //4MHz
            OSCCONbits.IRCF = 110;     
            break;            
    }
    OSCCONbits.SCS = 1; //se utiliza el reloj interno del sistema
}

/*******************************************************************************
 * Funciones para configurar timer 0 
 ******************************************************************************/
void conf_tmr0(uint8_t prescaler){
    switch(prescaler){              //1:2 rate
        case 0: 
            OPTION_REGbits.PS = 000;     
            break;   
        case 1:                     //1:4 rate
            OPTION_REGbits.PS = 001;     
            break;
        case 2:                     //1:8 rate
            OPTION_REGbits.PS = 010;     
            break;
        case 3:                     //1:16 rate
            OPTION_REGbits.PS = 011;     
            break;
        case 4:                     //1:32 rate
            OPTION_REGbits.PS = 100;     
            break;
        case 5:                     //1:64 rate
            OPTION_REGbits.PS = 101;     
            break;
        case 7:                     //1:256 rate
            OPTION_REGbits.PS = 111;     
            break;   
        default:                     //1:128 rate
            OPTION_REGbits.PS = 110;     
            break;            
    }
    OPTION_REGbits.T0CS = 0; //fuente del reloj interna Fosc/4  
    OPTION_REGbits.PSA = 0;  //preescalar asignado a TMR0  
}

/*******************************************************************************
 * Funciones para configurar ADC 
 ******************************************************************************/
void conf_ADC(uint8_t adcFrec, uint8_t isr, uint8_t Vref, uint8_t justRL){
    ADCON0bits.ADON = 1;
    
    switch(adcFrec){              
        case 0: 
            ADCON0bits.ADCS = 00;     //Fosc/2
            break;   
        case 1:                       //Fosc/8
            ADCON0bits.ADCS = 01;     
            break;
        case 2:                      //Fosc/32
            ADCON0bits.ADCS = 10;     
            break;
        case 3: 
            ADCON0bits.ADCS = 11; //FR
            break;
    }
    if (justRL == 0){
        ADCON1bits.ADFM = 0; //justificado a la izquierda
    }
    else{
        ADCON1bits.ADFM = 1; //justificado a la derecha
    }
    if (isr == 1) {
        PIE1bits.ADIE = 1; //Se habilita la interrupcion del ADC
        PIR1bits.ADIF = 0; //Se limpia la bandera del ADC
    }
    switch (Vref){
        case 0: //Vref+ pn y VSS
            ADCON1bits.VCFG0 = 1; 
            ADCON1bits.VCFG1 = 0; 
            break;
        case 1: //VDD y Vref - pin
            ADCON1bits.VCFG0 = 0; 
            ADCON1bits.VCFG1 = 1; 
            break;
        case 2: //Vref+ pin y Vref-pin
            ADCON1bits.VCFG0 = 1; 
            ADCON1bits.VCFG1 = 1;
            break;
        default: //Voltajes de referencia VSS y VDD
            ADCON1bits.VCFG0 = 0; 
            ADCON1bits.VCFG1 = 0; 
            break;    
    }
}

/*******************************************************************************
 * Funciones para conversión del ADC 
 ******************************************************************************/
void ADC_convert(char *data,float a, int place) 
{
     int temp=a;
     float x=0.0;
     int digits=0;
     int i=0,mu=1;
     int j=0;
     if(a<0)
     {
            a=a*-1;
            data[i]='-';
            i++;
      }
     //exponent component
     while(temp!=0)
     {
         temp=temp/10;
         digits++;          
     }
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }
     //mantissa component
     data[i]='.';
     i++;
     digits=0;
     for(j=1;j<=place;j++)mu=mu*10;
     x=(a-(int)a)*mu; //shift places
     a=x;
     temp=a;
     x=0.0;
     mu=1;
     digits=place;
     while(digits!=0)
     {
         if(digits==1)mu=1;
         else  for(j=2;j<=digits;j++)mu=mu*10;
         
         x=a/mu;
         a=a-((int)x*mu);
         data[i]=0x30+((int)x);
         i++;
         digits--;
         mu=1;
     }   
     
    data[i]='\n';
}

/*******************************************************************************
 Habilitar canales de conversión ADC
 ******************************************************************************/
void canal_ADC(uint8_t channel) { //Habilita los canales del ADC 
    switch (channel) {
        case 0: //Analogico en pin RA0
            ANSELbits.ANS0 = 1;
            break;
        case 1: //Analogico en pin RA1
            ANSELbits.ANS1 = 1;
            break;
        case 2: //Analogico en pin RA2
            ANSELbits.ANS2 = 1;
            break;
        case 3: //Analogico en pin RA3
            ANSELbits.ANS3 = 1;
            break;
        case 4: //Analogico en pin RA4
            ANSELbits.ANS4 = 1;
            break;
        case 5: //Analogico en pin RA5
            ANSELbits.ANS5 = 1;
            break;
        case 6: //Analogico en pin RA6
            ANSELbits.ANS6 = 1;
            break;
        case 7: //Analogico en pin RA7
            ANSELbits.ANS7 = 1;
            break;
        case 8: //Analogico en pin RB2
            ANSELHbits.ANS8 = 1;
            break;
        case 9: //Analogico en pin RB3
            ANSELHbits.ANS9 = 1;
            break;
        case 10: //Analogico en pin RB1
            ANSELHbits.ANS10 = 1;
            break;
        case 11: //Analogico en pin RB4
            ANSELHbits.ANS11 = 1;
            break;
        case 12: //Analogico en pin RB0
            ANSELHbits.ANS12 = 1;
            break;
        case 13: //Analogico en pin RB5
            ANSELHbits.ANS13 = 1;
            break;
    }
}

// Seleccionar Canal de Conversion
void Select_ch(uint8_t channel) {// se selecciona el canal analogico y se inicia la conversion
    switch (channel) {
        case 0: // AN0
            ADCON0bits.CHS = 0000;
            break;
        case 1: // AN1
            ADCON0bits.CHS = 0001;
            break;
        case 2: // AN2
            ADCON0bits.CHS = 0010;
            break;
        case 3: // AN3
            ADCON0bits.CHS = 0011;
            break;
        case 4: // AN4
            ADCON0bits.CHS = 0100;
            break;
        case 5: // AN5
            ADCON0bits.CHS = 0101;
            break;
        case 6: // AN6
            ADCON0bits.CHS = 0110;
            break;
        case 7: // AN7
            ADCON0bits.CHS = 0111;
            break;
        case 8: // AN8
            ADCON0bits.CHS = 1000;
            break;
        case 9: // AN9
            ADCON0bits.CHS = 1001;
            break;
        case 10: // AN10
            ADCON0bits.CHS = 1010;
            break;
        case 11: // AN11
            ADCON0bits.CHS = 1011;
            break;
        case 12: // AN12
            ADCON0bits.CHS = 1100;
            break;
        case 13: // AN13
            ADCON0bits.CHS = 1101;
            break;
        case 14: // CVref
            ADCON0bits.CHS = 1110;
            break;
        case 15: // Fixed ref
            ADCON0bits.CHS = 1111;
            break;
    }
    __delay_us(200); //es el tiempo recomendado de espera por cada cambio de canal
    ADCON0bits.GO = 1;
    
}