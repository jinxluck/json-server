/*
 * adc_temp.h
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#ifndef ADC_TEMP_H_
#define ADC_TEMP_H_
//adc header file

/*
 * read_adc/temp
 * 	check adc value
 * 	global_variable = adc value
 */
void READ_TEMP(void);

/*
 * return_temp
 * 	convert adc value
 * 	return converted value
 */
float RETURN_TEMP(void);



#endif /* ADC_TEMP_H_ */
