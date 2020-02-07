/*
 * timer.h
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#ifndef TIMER_H_
#define TIMER_H_

/*init_setitimer
 * timer takes seconds and milli seconds as argument
 * timer returns nothing
 * timer used for ISR
 */
void INIT_TIMER(int s, int ms);

/*timer_test
 *  test function to std output
 *  test's 4 runs
 */
void TIMER_TEST(void);

#endif /* TIMER_H_ */
