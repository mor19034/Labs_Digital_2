/* 
 * File:   EUSART.c
 * Author: pablo
 *
 * Created on 26 de julio de 2021, 19:18
 */
#include <xc.h>
#include "EUSART.h"

//******************************************************************************
//esta función sirve para inicializar transmisión asíncrona
//******************************************************************************
void init_USART (void){
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
}

void USART_Transmit(char dato){       //enviar datos 
    while(TXSTAbits.TRMT == 0);
    TXREG = dato;
}

char USART_Recieve(){                //Leer datos
    return RCREG; 
   }

void USART_Cadena(char *str){   //Envio de cadena de caracteres
    while(*str != '\0'){
        USART_Transmit(*str);
        str++;
    }
}
