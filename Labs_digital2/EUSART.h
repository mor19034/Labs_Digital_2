/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EUSART_H
#define	EUSART_H

//************************************librerias*********************************
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>

//**********************función de librería de EUSART***************************
void init_USART (void);
char USART_Recieve(void);
void USART_Cadena(char *str);
void USART_Transmit(char dato);

#endif	/* EUSART_H */

