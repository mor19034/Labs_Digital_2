/* 
 * File:   Lab04_master.c
 * Author: pablo
 *
 * Created on 7 de agosto de 2021, 13:28
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
#include "ADC.h"
#include "configuraciones_pic.h"
#include "LCD.h"

#define _XTAL_FREQ 8000000 

//**********Variables***********
volatile uint8_t var_adc0 = 0;
float cont_uart = 0;
char string_uart[10];
char valor_uart = 0;
char adc0[10];
char contador_lcd[10];
float conv0 = 0;
uint8_t contador;
 //**********Prototipos*********
 void setup(void);

 //**********Interrupcion********* 
//********************************loop principl*********************************
void main(void) {
    setup(); 
    Lcd_Init(); //se inisiliza la LCD  
    Lcd_Clear(); //se limpia la LCD
    while (1) {
         
        Lcd_Set_Cursor(1, 1); //primeras cordenadas de la pantalla
        Lcd_Write_String("S1:"); //se escribe en la pantalla
        Lcd_Set_Cursor(1, 8); //nos despalazamos en la pantalla
        Lcd_Write_String("S2:"); //se escribe de nuevo
        Lcd_Set_Cursor(1, 15); //nos desplazamos de nuevo
        Lcd_Write_String("S3:"); //y volvemos a escribir
        //****************se hace la cmunicación I2C con esclavo 1************** 
         I2C_Master_Start();
         I2C_Master_Write(0x11); //va a esclavo con direccion 0x50 y le dice que
                                 //va a leer
         var_adc0 = I2C_Master_Read(0);
         I2C_Master_Stop();
         __delay_us(200);
         
         I2C_Master_Start();
         I2C_Master_Write(0x21); //va a esclavo con direccion 0x50 y le dice que
                                 //va a leer
         contador = I2C_Master_Read(0);
         I2C_Master_Stop();
         __delay_us(200);

         
        //*********************se escribe en la LCD*****************************
        Lcd_Set_Cursor(2, 1); //nos colocamos abajo de V1 
        Lcd_Write_String(adc0); //mandamos el valor de la conversion en voltaje
        Lcd_Set_Cursor(2, 5);
        Lcd_Write_String("V");

        Lcd_Set_Cursor(2, 7);
        Lcd_Write_String(contador_lcd);
        Lcd_Set_Cursor(2, 11);
        Lcd_Write_String("U");
//        
//        Lcd_Set_Cursor(2,14);
//        Lcd_Write_String(cont);
        
        conv0 = 0;//se reinicia las cada ves que se inicia el proceso de enviar datos
        conv0 = (var_adc0 / (float) 255)*5; //Se consigue el porcentaje con 
        //respecto al valor maximo que un puerto puede tener, despues se 
        //multiplica por 5 para conocer el voltaje actual del puerto                                          
        ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.
        
        ADC_convert(contador_lcd, contador, 2); 
        }      
    return;
}
void setup(void){
    ANSEL = 0x00;
    ANSELH = 0x00;
    
    TRISA = 0x00;
    TRISB = 0x00;
    TRISE = 0x00;
    
    PORTA = 0X00;
    PORTB = 0x00;
    PORTE = 0x00;
    
    I2C_Master_Init(100000);        // Inicializar Comuncación I2C
    
    //Configuracion del Oscilador
    conf_osc(7); //8MHz
}

