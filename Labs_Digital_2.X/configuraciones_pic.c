/* 
 * File:   configuraciones_pic.c
 * Author: pablo moreno
 *
 * Created on 15 de julio de 2021, 12:30
 */
#include <stdio.h>
#include <stdlib.h>
#include "configuraciones_pic.h"

//-----------------------------variables----------------------------------------
uint8_t ADC_valor;
uint8_t upper_nibbles;
uint8_t down_nibbles;

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

void conf_ADC(uint8_t adcFrec){
    switch(adcFrec){              
        case 0: 
            ADCON0bits.ADCS = 00;     //Fosc/2
            break;   
        case 1:                       //Fosc/8
            ADCON0bits.ADCS = 01;     
            break;
        default:                      //Fosc/32
            ADCON0bits.ADCS = 10;     
            break;
    }
//  ADCON1bits.ADFM = 0; //Leer bits más significativos; justificado izquierda
    ADCON1bits.VCFG0 = 0; //Voltajes de referencia VSS y VDD
    ADCON1bits.VCFG1 = 0;     
}

void display(uint8_t dato){
    PORTC = tabla[dato];
}

void ADC_convert(uint8_t dato){
    ADC_valor = dato;
}

void nibbles (uint8_t dato){    //funcion para recibir datos del ADC
    down_nibbles = dato&0x0F;       //Pasan bits menos significativos
    upper_nibbles = (dato>>4)&0x0F; //paso a bits mas significativos
}