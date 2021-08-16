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
#include <stdio.h>
#include <stdlib.h>
#include <xc.h> // include processor files - each processor file is guarded.  
#include <pic16f887.h>
#include <stdint.h>
//**********************************definiciones********************************
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif

//**********************función de librería de EUSART***************************
void putch(uint8_t dato);

#endif	/* EUSART_H */

