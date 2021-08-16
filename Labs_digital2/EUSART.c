/* 
 * File:   EUSART.c
 * Author: pablo
 *
 * Created on 26 de julio de 2021, 19:18
 */
#include "EUSART.h"

//******************************************************************************
//esta función permite mandar strings por EUSART
//******************************************************************************
void putch(char dato){ 
    while(TXIF == 0);
    TXREG = dato; //lo que se escribe se manda al pic para que lo procese 
    return; 
}
//******************************************************************************
//esta función sirve para inicializar transmisión asíncrona
//******************************************************************************
void init_UART (void){
    TXSTAbits.SYNC = 0;     //modo sincrono desabilitado
    RCSTAbits.SPEN = 1;     //RX y TX habilitados como puertos seriales
    RCSTAbits.RX9 = 0;      //No se van a recibir 9 bits
    TXSTAbits.BRGH = 1;     //Bound rate High
    BAUDCTLbits.BRG16 = 1;  //16 bits para generar fire brake
    
}