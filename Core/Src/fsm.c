/*
 * fsm.c
 *
 *  Created on: Nov 15, 2023
 *      Author: PC
 */

#include "fsm.h"
#include "software_timer.h"
#include "main.h"
#include <stdio.h>

uint32_t ADC_value = 0;
uint8_t temp = 0;
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;
uint8_t buffer[MAX_BUFFER_SIZE];
int command_flag = 0;
uint8_t command_data[MAX_BUFFER_SIZE];
int cp_state = WAIT;
int cm_state = RST_WAITING;
char str[100];

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;

void clear_buffer() {
	memset(buffer,0,sizeof(buffer));
	index_buffer=0;
}

void command_parser_fsm() {
	switch(cp_state){
	case WAIT:
		if(temp == '!'){
			clear_buffer();
			cp_state = RECEIVE;
		}
		break;
	case RECEIVE:
		if(temp == '!'){
			clear_buffer();
		}
		else if(temp == '#'){
			command_flag = 1;
			memcpy(command_data, buffer, sizeof(command_data));
			clear_buffer();
		}
		else if(index_buffer == 0){
			cp_state = WAIT;
		}
		break;
	default:
		break;
	}
}

int isRSTreceived(){
	if(command_data[0] == 'R' && command_data[1] == 'S' && command_data[2] == 'T' && command_data[3] == '#')
		return 1;
	return 0;
}

int isOKreceived(){
	if(command_data[0] == 'O' && command_data[1] == 'K' && command_data[2] == '#')
		return 1;
	return 0;
}



void uart_communication_fsm() {
	switch(cm_state){
	case RST_WAITING:
		if(isRSTreceived()){
			HAL_ADC_Start(&hadc1);
			ADC_value = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			cm_state = SENDING;

		}
		break;
	case SENDING:
		if(timer1_flag == 1){
			if(isOKreceived()){
						clear_buffer();
						command_flag = 0;
						cp_state = WAIT;
						cm_state = RST_WAITING;
					}
			else{
				clear_buffer();
				memset(command_data, 0, sizeof(command_data));
				HAL_UART_Transmit(&huart2, (void*)str, sprintf(str, "!ADC=%d# \r\n", ADC_value), 1000);

			}
			HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
			setTimer1(300);
		}
		break;
	default:
		break;
	}
}


