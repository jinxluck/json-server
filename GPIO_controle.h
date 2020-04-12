/*
 * GPIO_controle.h
 *
 *  Created on: 20 Mar 2020
 *      Author: jinxluck
 */

#ifndef GPIO_CONTROLE_H_
#define GPIO_CONTROLE_H_

void init_gpio(int GPIO);

/*
 * direction:
 *  out = 1
 * 	in = 0
 *
 * 	GPIO:
 * 	 gpio number
 */
void set_direction(int GPIO, int direction);

/*
 * value:
 *  high = 1
 *  low = 0
 *
 * GPIO:
 *  gpio number
 */
void set_value(int GPIO, int value);

/*
 * GPIO:
 *  gpio number
 *
 * return value:
 *  the value of gpio XX
 */
int read_value(int GPIO);

#endif /* GPIO_CONTROLE_H_ */
