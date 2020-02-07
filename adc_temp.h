/*
 * adc_temp.h
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#ifndef ADC_TEMP_H_
#define ADC_TEMP_H_
/*adc header file
 *
 * init_adc
 * 	setup gpio's through fx. system()
 * 	void return
 * 	void arguments
*/
void INIT_ADC(void);

/*
 * read_adc/temp
 * 	check adc value
 * 	convert value
 * 	global_variable = converted value
 */
void READ_TEMP(void);

/*
 * return_temp
 * 	return global_variable
 */
float RETURN_TEMP(void);



#endif /* ADC_TEMP_H_ */
