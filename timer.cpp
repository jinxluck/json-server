/*
 * timer.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */
#include "adc_temp.h"
#include <stdio.h>
#include <iostream>
using namespace std;

//timer and handler includes
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <syslog.h>

int test_var = 0;

/*ISR handler
 * controlled by timer
 * read_adc/temp()
 */
void ISR_handler(int signum)
{
	READ_TEMP();
}


/*init_setitimer
 * timer takes seconds as argument
 * timer returns nothing
 * timer used for ISR
 */
void INIT_TIMER(int s, int ms)
{
	struct sigaction sa;
	struct itimerval timer;

	//install signal handler for SIGVTALRM
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &ISR_handler;
	int ret = sigaction(SIGVTALRM, &sa, NULL);
	if(ret)
	{
		perror("could not install signal handler");
		exit(EXIT_FAILURE);
	}

	//configure the timer
	//time
	timer.it_value.tv_sec = s;
	timer.it_value.tv_usec = ms*10000;
	//interval timer setting
	timer.it_interval.tv_sec = s;
	timer.it_interval.tv_usec = ms*10000;
	//start virtual timer count down
	ret = setitimer(ITIMER_VIRTUAL, &timer, NULL);
	if(ret)
	{
		perror("could not install timer");
		exit(EXIT_FAILURE);
	}

	while(1);
}

/*timer_test
 *  test function to std output
 *  tests 4 2sec run's
 */
void test_handler(int signum)
{
	test_var += 1;
	syslog(LOG_NOTICE, "Timer test");
}

void TIMER_TEST(void)
{
	struct sigaction sa;
	struct itimerval timer;

	//install signal handler for SIGVTALRM
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = &test_handler;
	int ret = sigaction(SIGVTALRM, &sa, NULL);
	if(ret)
	{
		perror("could not install signal handler");
		exit(EXIT_FAILURE);
	}

	//configure the timer
	//time
	timer.it_value.tv_sec = 2;
	timer.it_value.tv_usec = 0;
	//interval timer setting
	timer.it_interval.tv_sec = 2;
	timer.it_interval.tv_usec = 0;
	//start virtual timer count down
	ret = setitimer(ITIMER_VIRTUAL, &timer, NULL);
	if(ret)
	{
		perror("could not install timer");
		exit(EXIT_FAILURE);
	}

	while(test_var < 4);

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
}
