/*
 * server.cpp
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 *      TO DO:
 *      	get temp command
 *      	fork on new connection
 */
//user made headers
#include "server.h"
#include "adc_temp.h"
#include "GPIO_controle.h"

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

//json libraries
#include <document.h>
#include <writer.h>
#include <stringbuffer.h>
#include <iostream>

//global variable for heat controlling (obsolete for now)
//int desired_temp = 0;

#define PORT 1955

using namespace rapidjson;
using namespace std;


/*
 * Client thread:
 * controls the client handling, for new clients
 *
 * functions:
 * GETTEMP: returns the temperature
 * QUIT: closes the thread
 * ERROR: if the inbound message doesn't confort to the correct format,
 * 		  then an error message will be returned
 */
void *client_thread(void *client_func)
{
/*------------------------- varables used in thread ------------------------------------------------------- */
	syslog(LOG_NOTICE, "Client thread started");
	int new_socket, msgrecv;
	new_socket = *(int *) client_func;
	char buffer[1024] = {0};
	char msg[1024] =  {0};
	char jsonMessage[100];
	float temp_val;

/*-------------------------------- JSON setup --------------------------------------------------------------*/
    const char * json_out =
    		"{"
    		"\"command\":\"none\","
    		"\"temp\":0,"
    		"\"message\":\"none\""
    		"}";

    Document d_out, d_in;
    d_out.Parse(json_out);

/*--------------------------- eternal loop handling --------------------------------------------------------*/
	while(1)
	{
/*------------------------------ message recieving --------------------------------------------------------*/
		if ((msgrecv = recv(new_socket, buffer, 1000, 0)) == 0) {
			//if client socket is closed, without QUIT command

			//syslog notice
			syslog(LOG_NOTICE, "daemon server: Bad client disconnect!");

			//close socket and thread
			close(new_socket);
			return 0;
		} else {
			//if client socket is open, and message recieves

			//syslog notice
			syslog(LOG_NOTICE, "daemon server: message received");
			//syslog notice (see inbound message)
			syslog(LOG_NOTICE, buffer);
		}

		//set into json object
		d_in.Parse(buffer);
		//clear buffer
		memset(buffer, '\0', strlen(buffer));

/*------------------------------ JSON structure check ---------------------------------------------------*/
		bool check1 = 0;
		bool check2 = 0;
		bool check3 = 0;
		if(!(check1 = d_in.HasMember("command")))
			syslog(LOG_NOTICE, "message error: structure missing \"command\"");
		if(!(check2 = d_in.HasMember("temp")))
			syslog(LOG_NOTICE, "message error: structure missing \"temp\"");
		if(!(check3 = d_in.HasMember("message")))
			syslog(LOG_NOTICE, "message error: structure missing \"message\"");
		if(check1 && check2 && check3) //if structure is correct
		{
			//get command
			strncpy(msg, d_in["command"].GetString(), strlen(d_in["command"].GetString()));

/*--------------------------------- GETTEMP function ---------------------------------------------------*/
			if(strncmp(msg, "GETTEMP", strlen("GETTEMP")) == 0)
			{
				//get temp
				temp_val = RETURN_TEMP();

				//convert temp(float) to temp(int)
				//insert temp into json doc
				Value& s = d_out["temp"];
				s = (int)temp_val;

				//return messages
				d_out["command"].SetString("GETTEMP");
				d_out["message"].SetString("temp read");

				//Convert json doc to Json string
				StringBuffer jsonBuffer;
				Writer<StringBuffer> writer(jsonBuffer);
				d_out.Accept(writer);

				//send json string
				send(new_socket, jsonBuffer.GetString(), strlen(jsonBuffer.GetString()), 0);
			}
/*------------------------------------- QUIT function --------------------------------------------------*/
			else if(strncmp(msg, "QUIT", strlen("QUIT")) == 0)
			{
				//syslog notice
				syslog(LOG_NOTICE, "Client closed: QUIT command given");

				//insert messages into JSON doc
				d_out["message"].SetString("Goodbye");
				d_out["command"].SetString("QUIT");

				//Convert json doc to Json string
				StringBuffer jsonBuffer;
				Writer<StringBuffer> writer(jsonBuffer);
				d_out.Accept(writer);

				//send JSON string
				send(new_socket, jsonBuffer.GetString(), strlen(jsonBuffer.GetString()), 0);

				//close socket and thread
				close(new_socket);
				return 0;
			}
		}
		else
/*------------------------------------- ERROR function --------------------------------------------------*/
		{
			//syslog notice
			syslog(LOG_NOTICE, "Client closed: Json structure error");

			//insert messages into JSON doc
			d_out["message"].SetString("Json structure error");
			d_out["command"].SetString("ERROR");

			//Convert json doc to Json string
			StringBuffer jsonBuffer;
			Writer<StringBuffer> writer(jsonBuffer);
			d_out.Accept(writer);

			//send JSON string
			send(new_socket, jsonBuffer.GetString(), strlen(jsonBuffer.GetString()), 0);

			//close socket and thread
			close(new_socket);
			return 0;
		}
		//wait for 1 second
		sleep(1);
	}
	//obsolete
	return 0;
}

/*
 * Socket thread:
 * looks for inbound clients
 *
 * incase of new client -> start new client handling thread
 */
void *socket_thread(void *socket_func)
{
/*------------------------- varables used in thread ------------------------------------------------------- */
	int server_fd, new_socket;
	struct sockaddr_in adress;
	int opt = 1;
	int addrlen = sizeof(adress);
	//greeting (obsolete)
	/*
	const char *greeting = "Welcome to daemon server \n QUIT to close connection "
			"\n GET TEMP to get the temperature \n SET TEMP XX to set temp ("
			"replace XX with integer fx: 01 or 12\n";

	*/

/*-------------------------socket handling ------------------------------------------------------- */
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

/*------------------------- customer service ------------------------------------------------------- */
	while (1)
	{
		//listen for new clients
		if (listen(server_fd, 1) < 0)
		{
			syslog(LOG_NOTICE, "daemon server: server listen failed");
			exit(EXIT_FAILURE);
		}

		//new client discovered
		if ((new_socket = accept(server_fd, (struct sockaddr *) &adress,
				(socklen_t*) &addrlen)) < 0) {
			syslog(LOG_NOTICE, "Daemon server: client not accepted");
		} else {
			syslog(LOG_NOTICE, "Daemon server: client accepted");

			//create client handling thread
			pthread_t client_id;
			pthread_create(&client_id, NULL, client_thread, &new_socket);
		}
		//sleep for 1 second (obsolete)
		sleep(1);
	}

	return NULL;
}

/*
 * heat controling thread
 *
 * obsolete for now
 *
void *heat_controler(void *heat_func)
{
	int GPIO_OUT = 52; //P2.10
	int GO_pin = 210; //set to desired gpio

	int ret;

	init_gpio(GO_pin);
	set_direction(GPIO_OUT, 1);

	while(1)
	{
		if((ret = RETURN_TEMP()) > desired_temp)
		{
			set_value(GPIO_OUT, 0);
		}
		else
		{
			set_value(GPIO_OUT, 1);
		}

		sleep(1);
	}

	return 0;
}
*/

/*
 * Server initialising function:
 * starts threads for:
 * server handling
 * heat controlling (obsolete for now)
 */
void INIT_SERVER(void)
{
	pthread_t thread_id/* ,thread_id2*/;

	//server thread
	pthread_create(&thread_id, NULL, socket_thread, NULL);

	//heat controller thread (obsolete for now)
	//pthread_create(&thread_id2, NULL, heat_controler, NULL);
}

