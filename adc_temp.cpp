/*
 * adc_temp.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */
#include "adc_temp.h"
#include <string.h>
#include <syslog.h>
#include <stdio.h>

#define AIN_path "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"

volatile int LAST_ADC_VAL;


/*
 * read_adc/temp
 * 	check adc value
 * 	global_variable = adc value
 */
void READ_TEMP(void)
{
	FILE *fs;

	fs = fopen(AIN_path, "r");
	if(!fs)
	{
		syslog(LOG_NOTICE, "temperature reading failed!");
	}

	fscanf(fs, "%d", &LAST_ADC_VAL);
	fclose(fs);
}


 /*
 * return_temp
 * 	convert adc value
 * 	return converted value
*/
float RETURN_TEMP(void)
{
	float temp = 0;

	temp = (LAST_ADC_VAL*1.8)/4095;
	temp = temp/0.01;

	return temp;
}

