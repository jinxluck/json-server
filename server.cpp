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
//#include adc header (for return temp function!)

//shared libraries
#include <stdlib.h>
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
void *socket_thread(void *socket_func) {
	int server_fd, new_socket, msgrecv;
	struct sockaddr_in adress;
	int opt = 1;
	int addrlen = sizeof(adress);
	//{0} sets whole string to 0's
	char buffer[20] = { 0 };
	//greeting
	const char *greeting = "Welcome to daemon server \n QUIT to close connection \n QUIT! to shutdown daemon \n";

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

	while (1) {
		if (listen(server_fd, 1) < 0) {
			syslog(LOG_NOTICE, "daemon server: server listen");
			exit(EXIT_FAILURE);
		}

		if ((new_socket = accept(server_fd, (struct sockaddr *) &adress,
				(socklen_t*) &addrlen)) < 0) {
			syslog(LOG_NOTICE, "Daemon server: client not accepted");
			exit(EXIT_FAILURE);
		} else {
			syslog(LOG_NOTICE, "Daemon server: client accepted");
			send(new_socket, greeting, strlen(greeting), 0);
		}

		while (1) {
			if ((msgrecv = recv(new_socket, buffer, 20, 0)) == -1) {
				syslog(LOG_NOTICE, "daemon server: message fail");
			} else {
				syslog(LOG_NOTICE, "daemon server: message recieved");
				send(new_socket, buffer, strlen(buffer), 0);
			}

			if (buffer[0] == 'Q') {
				if (buffer[1] == 'U') {
					if (buffer[2] == 'I') {
						if (buffer[3] == 'T') {
							break;
						}
					}
					{
						buffer[0] = '\0';
					}
				}
				{
					buffer[0] = '\0';
				}
			} else {
				buffer[0] = '\0';
			}
		}
		if((close(new_socket))==0)
		{
			syslog(LOG_NOTICE, "Client socket closed");
		}
		else
		{
			syslog(LOG_NOTICE, "Client closing failed");
		}
		if (buffer[4] == '!') {
			if((close(server_fd))==0)
			{
				syslog(LOG_NOTICE, "Server socket closed");
			}
			else
			{
				syslog(LOG_NOTICE, "Socket closing failed");
			}
			break;
		} else {
			buffer[0] = '\0';
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

	pthread_join(thread_id, NULL);
}

