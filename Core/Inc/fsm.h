/*
 * fsm.h
 *
 *  Created on: Nov 15, 2023
 *      Author: PC
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"
#include "software_timer.h"

#define WAIT	1
#define RECEIVE 2

#define INIT	0
#define RST_WAITING 1
#define SENDING 2
#define DEFAULT -1
#define MAX_BUFFER_SIZE 30

extern uint8_t temp;
extern uint8_t buffer[MAX_BUFFER_SIZE];
extern uint8_t index_buffer;
extern uint8_t buffer_flag;
extern uint32_t ADC_value;
extern int command_flag;

void command_parser_fsm();
void uart_communication_fsm();
void clear_buffer();

#endif /* INC_FSM_H_ */
