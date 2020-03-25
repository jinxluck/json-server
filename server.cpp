/*
 * server.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 *      TO DO:
 *      	get temp command
 *      	fork on new connection
 */
#include "server.h"
#include "adc_temp.h"

//shared libraries
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>

//thread libraries
#include <pthread.h>

//socket libraries
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1955

/*
 *  socket function
 * 	changes compared to former project:
 * 		if new connection is registered -> fork() or new string
 * 		in the new fork/string handle the new connection
 * 		has to take the command "GET TEMP" to read out temp from glob var
 */

void *client_thread(void *client_func)
{
	syslog(LOG_NOTICE, "Client thread started");
	int new_socket, msgrecv;
	new_socket = *(int *) client_func;
	char buffer[20] = {0};
	char return_msg[30] =  {0};
	float temp_val;

	while(1)
	{
		if ((msgrecv = recv(new_socket, buffer, 20, 0)) == -1) {
			syslog(LOG_NOTICE, "daemon server: message fail");
		} else {
			syslog(LOG_NOTICE, "daemon server: message recieved");
		}

		if(strncmp(buffer, "GET TEMP", strlen("GET TEMP")) == 0)
		{
			temp_val = RETURN_TEMP();
			snprintf(return_msg,10,"%f", temp_val);
			strcat(return_msg," \n");

			send(new_socket, return_msg, strlen(return_msg), 0);

			memset(buffer, 0, strlen(buffer));
			memset(return_msg, 0, strlen(return_msg));
		}
		else if(strncmp(buffer, "QUIT", strlen("QUIT")) == 0)
		{
			memset(buffer, 0, strlen(buffer));
			syslog(LOG_NOTICE, "Client kill");
			close(new_socket);
			return 0;
		}
		else
		{
			strncpy(return_msg, "Wrong command... \n", strlen("Wrong command... \n"));
			send(new_socket, return_msg, strlen(return_msg), 0);
			memset(buffer, 0, strlen(buffer));
			memset(return_msg, 0, strlen(return_msg));
		}
	}
	return 0;
}

void *socket_thread(void *socket_func) {
	int server_fd, new_socket;
	struct sockaddr_in adress;
	int opt = 1;
	int addrlen = sizeof(adress);
	//greeting
	const char *greeting = "Welcome to daemon server \n QUIT to close connection "
			"\n GET TEMP to get the temperature \n";

	//create socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		syslog(LOG_NOTICE, "daemon server start: file descriptor failed");
		exit(EXIT_FAILURE);
	} else {
		syslog(LOG_NOTICE, "daemon server start: file descriptor success");
	}

	//attaching socket to port "rules"
	//reuseaddr and reuseport is allowed, opt is the boolean value
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
			sizeof(opt))) {
		syslog(LOG_NOTICE, "daemon server start: set socket options failed");
		exit(EXIT_FAILURE);
	} else {
		syslog(LOG_NOTICE, "daemon server start: set socket options success");
	}
	adress.sin_family = AF_INET;
	adress.sin_addr.s_addr = INADDR_ANY;
	adress.sin_port = htons(PORT);

	//attaching socket to port "bind"
	if (bind(server_fd, (struct sockaddr *) &adress, sizeof(adress)) < 0) {
		syslog(LOG_NOTICE, "daemon server start: bind failed");
		exit(EXIT_FAILURE);
	} else {
		syslog(LOG_NOTICE, "server start: bind success");
	}

	//customer service
	while (1) {
		if (listen(server_fd, 1) < 0) {
			syslog(LOG_NOTICE, "daemon server: server listen failed");
			exit(EXIT_FAILURE);
		}

		if ((new_socket = accept(server_fd, (struct sockaddr *) &adress,
				(socklen_t*) &addrlen)) < 0) {
			syslog(LOG_NOTICE, "Daemon server: client not accepted");
			exit(EXIT_FAILURE);
		} else {
			syslog(LOG_NOTICE, "Daemon server: client accepted");
			send(new_socket, greeting, strlen(greeting), 0);

			pthread_t client_id;
			pthread_create(&client_id, NULL, client_thread, &new_socket);
		}
	}

	return NULL;
}

/*
 * init_server
 * 	thread for socket
 * 	call socket function
 * 	return value?
 * 	arguments?
 */
void INIT_SERVER(void)
{
	pthread_t thread_id;

	pthread_create(&thread_id, NULL, socket_thread, NULL);
}

