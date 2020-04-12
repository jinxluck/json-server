/*
 * GPIO_controle.cpp
 *
 *  Created on: 20 Mar 2020
 *  Author: jinxluck
 *
 *  Description:
 *  	Controle driver for the gpio's on the pocket beagle
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GPIO_controle.h"

//Path to the gpio folder
char PATH[100] = "/sys/class/gpio/gpio";

/*
 * GPIO:
 *  fx 106 for gpio P1.06
 */
void init_gpio(int GPIO)
{
	//For visual feedback
	static int x = 1;
	if(x)
	{
		printf(" *Setting pins to gpio\n");
		x = 0;
	}

	//Buffer for the command string
	char command[50];
	memset(command, '\0', 50);

	//Buffer for the argument "gpio"
	char buffer[5];
	snprintf(buffer, 5, "%d", GPIO);

	//Concatenate string part's into the command buffer
	strcat(command, "config-pin P");
	command[12] = buffer[0];
	command[13] = '.';
	command[14] = buffer[1];
	command[15] = buffer[2];
	strcat(command, " gpio");

	//Parse command to the system
	system(command);

	printf("   |-%s\n",command);
}

/*
 * direction:
 *  out = 1
 * 	in = 0
 *
 * 	GPIO:
 * 	 gpio number
 */
void set_direction(int GPIO, int direction)
{
	//For visual feedback
	static int y = 1;
	if(y)
	{
		printf(" *Setting direction\n");
		y = 0;
	}

	//Create file pointer
	FILE *fs;

	//Buffer for the file path
	char dir_path[100];
	memset(dir_path, '\0', 100);

	//Buffer for the argument "GPIO"
	char buffer[5];
	snprintf(buffer, 5, "%d", GPIO);

	//Concatenate the strings into dir_path buffer
	strcat(dir_path, PATH);
	strcat(dir_path, buffer);
	strcat(dir_path, "/direction");

	//Open the "direction" file in write mode
	fs=fopen(dir_path, "w");
	//Set pointer to the start of the file
	fseek(fs, 0, SEEK_SET);

	//Change direction value, dependent on the argument "direction"
	if(direction == 0)
	{
		fprintf(fs,"in");
		printf("   |-GPIO: %d is now an input\n", GPIO);
	}
	else
	{
		fprintf(fs,"out");
		printf("   |-GPIO: %d is now an output\n", GPIO);
	}

	//Close file pointer
	fclose(fs);
}

/*
 * value:
 *  high = 1
 *  low = 0
 *
 * GPIO:
 *  gpio number
 */
void set_value(int GPIO, int value)
{
	//Create file pointer
	FILE *fs;

	//Buffer for the file path
	char dir_path[100];
	memset(dir_path, '\0', 100);

	//Buffer for the argument "GPIO"
	char buffer[5];
	snprintf(buffer, 5, "%d", GPIO);

	//Buffer for the argument "value"
	char val[5];
	snprintf(val, 5, "%d", value);

	//Concatenate the strings into "dir_path" buffer
	strcat(dir_path, PATH);
	strcat(dir_path, buffer);
	strcat(dir_path, "/value");

	//Open the file "value" in write mode
	fs=fopen(dir_path, "w");
	//Set pointer to the start of the file
	fseek(fs, 0, SEEK_SET);

	//Overwrite current value
	fprintf(fs, val);

	//Close file pointer
	fclose(fs);
}

/*
 * GPIO:
 *  gpio number
 *
 * return value:
 *  the value of gpio XX
 */
int read_value(int GPIO)
{
	//Create file pointer
	FILE *fs;

	//Buffer for the file path
	char dir_path[100];
	memset(dir_path, '\0', 100);

	//Variable for return value
	int value;

	//Buffer for the argument "GPIO"
	char buffer[5];
	snprintf(buffer, 5, "%d", GPIO);

	//Concatenate the strings into "dir_path" buffer
	strcat(dir_path, PATH);
	strcat(dir_path, buffer);
	strcat(dir_path, "/value");

	//Open the file "value" in read mode
	fs=fopen(dir_path, "r");
	//Set pointer to the start of the file
	fseek(fs, 0, SEEK_SET);

	//Copy the value to the variable value
	fscanf(fs,"%d",&value);

	//Close file pointer
	fclose(fs);

	//Return value of "value"
	return value;
}







