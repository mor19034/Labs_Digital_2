/* 
 * File:   
 * Author: Pablo Moreno
 * Comments: esta es mi libreria SIIIU
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __HEADER_TEMPLATE_H_
#define	__HEADER_TEMPLATE_H_

#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

void conf_osc(uint8_t frec);
void conf_tmr0(uint8_t prescaler);

#endif	/* XC_HEADER_TEMPLATE_H */

