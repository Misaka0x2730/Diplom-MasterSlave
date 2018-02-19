#ifndef __F107_H
#define __F107_H

#include "stm32f10x_conf.h"

#define	F_APB1 36000000

static unsigned long *UID;

void delay_ms (unsigned int delay);
void delay_us (unsigned int delay);

#endif //__F107_H
