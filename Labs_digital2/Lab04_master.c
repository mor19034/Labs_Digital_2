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
//*******************************definiciones***********************************
#define _XTAL_FREQ 8000000 
#define Sample_Rate_Divider 0x19
#define CONFIG_MPU6050 0x1A
#define Gyro_CONFIG 0x1B
#define Accel_CONFIG 0x1C
#define Power_Managment_1 0x6B
#define Accel_Xout_H 0x3B
#define MPU_Write 0xD0
#define MPU_Read 0xD1

//**********Variables***********
volatile uint8_t var_adc0 = 0;
float cont_uart = 0;
char string_uart[10];
char valor_uart = 0;
char adc0[10];
char contador_lcd[10];
char sensor_lcd[10];
float conv0 = 0;
uint8_t contador;
int8_t sensor;
 //**********Prototipos*********
 void setup(void);
 void I2C_MPU_Init(void);
 void I2C_Read_MPU(float* data_send);
 //**********Interrupcion********* 
//********************************loop principl*********************************
void main(void) {
    setup(); 
    Lcd_Init(); //se inisiliza la LCD  
    Lcd_Clear(); //se limpia la LCD
    void I2C_MPU_Init(void);
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

        //I2C sensor 
        I2C_Master_Start();
        I2C_Master_Write(0x9A);
        I2C_Master_Write(0x00);
        __delay_ms(100);
        I2C_Master_Stop();
        __delay_ms(200);
        
        I2C_Master_Start();
        __delay_ms(200);
        I2C_Master_Write(0x9B);
        sensor= I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //*********************se escribe en la LCD*****************************
        Lcd_Set_Cursor(2, 1); //nos colocamos abajo de V1 
        Lcd_Write_String(adc0); //mandamos el valor de la conversion en voltaje
        Lcd_Set_Cursor(2, 5);
        Lcd_Write_String("V");

        Lcd_Set_Cursor(2, 7);
        Lcd_Write_String(contador_lcd);
        Lcd_Set_Cursor(2, 11);
        Lcd_Write_String("U");
        
        Lcd_Set_Cursor(2,14);
        Lcd_Write_String(sensor_lcd);
        
        conv0 = 0;//se reinicia las cada ves que se inicia el proceso de enviar datos
        conv0 = (var_adc0 / (float) 255)*5; //Se consigue el porcentaje con 
        //respecto al valor maximo que un puerto puede tener, despues se 
        //multiplica por 5 para conocer el voltaje actual del puerto                                          
        ADC_convert(adc0, conv0, 2);//se convierte el valor actual a un valor ASCII.
        
        ADC_convert(contador_lcd, contador, 2); 
        ADC_convert(sensor_lcd, sensor, 2);
        }      
    return;
}
//********************************configuración*********************************
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

//*************************************funciones********************************
//void I2C_MPU_Init(void) {
//    //Config del modo de energia y reloj 
//    I2C_Master_Start();
//    I2C_Master_Write(MPU_Write); //Direccion de escritura 0xD0
//    I2C_Master_Write(Power_Managment_1); //Es el para el registro con direccion 0x6B
//    I2C_Master_Write(0x01); //PLL ref en el eje x
//    I2C_Master_Stop();
//    //Config de la frecuencia de los datos
//    I2C_Master_Start();
//    I2C_Master_Write(MPU_Write); //Direccion de escritura
//    I2C_Master_Write(Sample_Rate_Divider); //Es el para el registro con direccion 0x19
//    I2C_Master_Write(0x07); //Es el para que los datos tengan una frecuencia de 1KHz
//    I2C_Master_Stop();
//    //Config general 
//    I2C_Master_Start();
//    I2C_Master_Write(MPU_Write); //Direccion de escritura
//    I2C_Master_Write(CONFIG_MPU6050); //Es el para el registro con direccion 0x1A
//    I2C_Master_Write(0x00); //Input desactivada, maxima ancho de banda para el accel
//    I2C_Master_Stop();
//    //Config del giroscopio
//    I2C_Master_Start();
//    I2C_Master_Write(MPU_Write); //Direccion de escritura
//    I2C_Master_Write(Gyro_CONFIG); //Es el para el registro con direccion 0x1B
//    I2C_Master_Write(0x00); //Sin pruebas, +-250°/s  
//    I2C_Master_Stop();
//    //Config del acelerometro
//    I2C_Master_Start();
//    I2C_Master_Write(MPU_Write); //Direccion de escritura
//    I2C_Master_Write(Accel_CONFIG); //Es el para el registro con direccion 0x1C
//    I2C_Master_Write(0x00); //Sin pruebas y escala de +-2g
//    I2C_Master_Stop();
//    return;
//}
//
//void I2C_Read_MPU(float* data_send) {
//    //En esta funcion solo se recuperan 6 de los 14 registros que tiene el sensor
//    
//    char temp[6]; //valores temporales
//    int valor_original[3]; // arreglo donde se van a guardar los datos 
//    
//    I2C_Start(MPU_Write);
//    while (SSPCON2bits.ACKSTAT);
//    I2C_Master_Write(Accel_Xout_H); //Registro donde se inicia la lectura de valores
//    while (SSPCON2bits.ACKSTAT);
//    I2C_Master_RepeatedStart();
//    I2C_Master_Write(MPU_Read);
//    for (int Addr = 0; Addr < 5; Addr++) temp[Addr] = I2C_Read(0);
//    temp[5] = I2C_Read(1);
//    I2C_Master_Stop();
//    
//    //Registros de la aceleracion 
//    valor_original[0] = ((int) temp[0] << 8) | ((int) temp[1]); //en el eje x
//    valor_original[1] = ((int) temp[2] << 8) | ((int) temp[3]); //en el eje y
//    valor_original[2] = ((int) temp[4] << 8) | ((int) temp[5]); //en el eje z
//    //El valor se arregla para que los valores esten en m/s^2
//    data_send[0] = ((float) valor_original[0]) * 0.0005982; 
//    data_send[1] = ((float) valor_original[1]) * 0.0005982; //aceleracion en m/s^2
//    data_send[2] = ((float) valor_original[2]) * 0.0005982; //aceleracion en m/s^2
//    return;
//}