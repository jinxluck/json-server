/*
 * daemon.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

//daemon libraries
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include "daemon.h"

void INIT_DAEMON(void)
{
	//pid_t is pid datatype for handling pid's
	pid_t pid;

	//fork the parent process
	pid = fork();

	//error checks:
	//if fork gives negative value, it was unsuccessful
	if (pid < 0)
	{
		syslog(LOG_NOTICE, "Daemon parent fork failure");
		exit(EXIT_FAILURE);
	}

	//if fork gives positive value, it was successful
	//if successful, kill the parent
	if (pid > 0)
	{
		syslog(LOG_NOTICE, "Daemon parent fork Success");
		exit(EXIT_SUCCESS);
	}

	//the child becomes session leader
	//returns -1 if error occurred
	if (setsid() < 0)
		exit(EXIT_FAILURE);

	//fork a second time to ensure no zombies
	pid = fork();

	//error checks:
	//if fork gives negative value, it was unsuccessful
	if (pid < 0)
	{
		syslog(LOG_NOTICE, "Daemon child fork failure");
		exit(EXIT_FAILURE);
	}

	//if fork gives positive value, it was successful
	//if successful, kill the parent
	if (pid > 0)
	{
		syslog(LOG_NOTICE, "Daemon child fork Success");
		exit(EXIT_SUCCESS);
	}

	//set file permissions
	//umask(0) = file is world-writeable
	umask(0);

	//change working directory
	chdir("/");

	//close all open file descriptors
	//SC_OPEN_MAX is max open files for a process
	int x = 1;
	for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
	{
		close(x);
	}

	syslog(LOG_NOTICE, "The deamon is alive!");
}


