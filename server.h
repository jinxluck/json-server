/*
 * server.h
 *
 *  Created on: 7 Feb 2020
 *      Author: jinxluck
 */

#ifndef SERVER_H_
#define SERVER_H_

/*server init
 * has to be called last, because of the join thread function,
 * will halt the program indef till the program closes!
 */
void INIT_SERVER(void);


#endif /* SERVER_H_ */
