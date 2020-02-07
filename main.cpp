/*
 * main.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#include "daemon.h"
#include "server.h"
#include "timer.h"
#include "adc_temp.h"

/*init_setitimer
 * timer takes seconds as argument
 * timer returns nothing
 * timer used for ISR
 */

/*ISR handler
 * controlled by timer
 * read_adc/temp()
 */



int main(void)
{
	INIT_DAEMON();
	//init_adc
	//init_setitimer
	INIT_SERVER();

	return 0;
}
