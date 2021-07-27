/* 
 * File:   Lab02.c
 * Author: pablo
 *
 * Created on 25 de julio de 2021, 19:51
 */

#include <stdio.h>
#include <stdlib.h>
#include "configuraciones_pic.h"
#include "LCD.h"
#include <stdint.h>
#include <xc.h>// </editor-fold>

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

#define _XTAL_FREQ 800000

//------------------------------variables---------------------------------------
uint8_t canal_flag = 0;
volatile uint8_t var_adc0 = 0;
volatile uint8_t var_adc1 = 0;
float cont_uart = 0;
char string_uart[10];
char valor_uart = 0;
char adc0[10];
char adc1[10];
float conv0 = 0;
float conv1 = 0;

//------------------------------prototipos--------------------------------------
void setup (void);
void mensaje (void);
//----------------------------------loop princiapl------------------------------
void main(void){
    setup();
    Lcd_Init(); //se inisiliza la LCD
    canal_ADC(0); //Se habilita canal RA0 para ADC
    canal_ADC(1); //Se habilita canal RA1 para ADC
    Select_ch(0); //la conversion ADC empieza en canal 0 (RA0)
    Lcd_Clear(); //se limpia la LCD
    while(1){
        Lcd_Set_Cursor(1, 1); //primeras cordenadas de la pantalla
        Lcd_Write_String("S1:"); //se escribe en la pantalla
        Lcd_Set_Cursor(1, 8); //nos despalazamos en la pantalla
        Lcd_Write_String("S2:"); //se escribe de nuevo
        Lcd_Set_Cursor(1, 15); //nos desplazamos de nuevo
        Lcd_Write_String("S3:"); //y volvemos a escribir
        
        mensaje (); //se manda la info por UART
        
        if (PIR1bits.ADIF == 1) { //se revisa si hubo interrupcion del ADC
            if (canal_flag == 0) {
                var_adc0 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(0);
                canal_flag++;
            } else {
                var_adc1 = ADRESH; //se guarda el valor convertido en la variable
                Select_ch(1);
                canal_flag--;
            }
            PIR1bits.ADIF = 0;
        }
   
    Lcd_Set_Cursor(2, 1);
        Lcd_Write_String(adc0);
        Lcd_Set_Cursor(2, 5);
        Lcd_Write_String("V");

        Lcd_Set_Cursor(2, 7);
        Lcd_Write_String(adc1);
        Lcd_Set_Cursor(2, 11);
        Lcd_Write_String("V");
    
     conv0 = 0;//se reinicia las cada ves que se inicia el proceso de enviar datos
     conv1 = 0;//tanto para la LCD como por UART.
        
        conv0 = (var_adc0 / (float) 255)*5; //Se consigue el porcentaje con respecto al valor 
       //maximo que un puerto puede tener, despues se multiplica por 5 para conocer el voltaje actual del puerto                                          
        ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.
        
        conv1 = (var_adc1 / (float) 255)*5; //misma logica que conv0
        ADC_convert(adc1, conv1, 2);
    }
        
    return;
}
//*******************************funciones**************************************
void mensaje (void){
    __delay_(300);
    printf("\r voltaje 1: \r");
    __delay_ms(300);
    printf(adc0);
    __delay_ms(300);
    printf("\r voltaje 2: \r");
    __delay_ms(300);
    printf(adc1);
} 
void putch(char dato){ 
    while(TXIF == 0);
    TXREG = dato; //lo que se escribe se manda al pic para que lo procese 
    return; 
}
//----------------------------------Configuraciones-----------------------------
void setup (void){
    ANSEL = 0x00;
    ANSELH = 0;
    
    TRISA = 0x03;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    //------reloj oscilador
    conf_osc(7);
    
    //------configuracion ADC
    conf_ADC(2, 0, 3, 0);    /*Fosc/32 para tads de 4us, sin interrupciones 
                              voltajes VSS y VDD y justificado a la izquierda*/

    //------------conf EUSART
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
