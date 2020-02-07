/*
 * main.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#include "daemon.h"
#include "server.h"
#include "timer.h"
#include <signal.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>

void kill_sig(int sig, siginfo_t *siginfo, void *context)
{
	syslog(LOG_NOTICE, "Daemon kill!");

	exit(0);
}

int main(void)
{
	INIT_DAEMON();

	//TIMER_TEST();

	//signal setup:
	struct sigaction user_sig;

	//install sig. handler
	user_sig.sa_sigaction = &kill_sig;
	//use sa_sigaction instead of sa_handler
	user_sig.sa_flags = SA_SIGINFO;

	//install handler and relate to SIGTERM flag
	sigaction(SIGHUP, &user_sig, NULL);

	INIT_SERVER();

	INIT_TIMER(15,0);

	return 0;
}
