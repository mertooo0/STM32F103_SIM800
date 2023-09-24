#include <stdlib.h>
#include<SIM800.h>
#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdbool.h"

extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
//------Variables----------

extern char RxBuffer[25];

//-------------------------
//-------COMMANDS---------

char Command1[5]="AT\r\n",
	 Command2[]="AT+CPIN?\r\n",
	 Command3[]="AT+CREG?\r\n",
	 Command4[]="AT+CGATT?\r\n",
	 Command5[]="AT+CIPSHUT\r\n",
	 Command6[]="AT+CIPSTATUS\r\n",
	 Command7[]="AT+CIPMUX=0\r\n",
	 Command8[]="AT+CSTT=\"internet\"\r\n",
	 Command9[]="AT+CIICR\r\n",
	 Command10[]="AT+CIFSR\r\n",
	 Command11[]="AT+CIPSPRT=0\r\n",
	 Command12[]="AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"\r\n",
	 Command13[]="AT+CIPSEND\r\n",
	 Command14[]="GET https://api.thingspeak.com/update?api_key=I7HHP47YNWI8PSM8&field1=0\r\n",
	 Command15[]="24\r\n",
	 Command16[]="AT+CIPSHUT\r\n"
;
//------------------------

uint8_t test()
{
	uint8_t ans=0;
	memset(RxBuffer,0,sizeof(RxBuffer)*strlen(RxBuffer)); //yanlış nedir ?

	HAL_UART_Transmit(&huart1,(uint8_t*)Command1,strlen(Command1),100);//!!HUART1 TX VERİ GÖNDERMİYOR
	//HAL_UART_Receive(&huart1,(uint8_t*)RxBuffer,6, 1000);
	ans=Check_OK();
	return ans;
}


uint8_t test2()
{
	uint8_t ans=0;
	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command15,strlen(Command15),100);
	ans=Check_OK();
	return ans;
}

void test3()
{

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command1,strlen(Command1),100);
	HAL_Delay(50);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command2,strlen(Command2),100);
	HAL_Delay(50);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command3,strlen(Command3),100);
	HAL_Delay(50);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command4,strlen(Command4),100);
	HAL_Delay(50);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command5,strlen(Command5),100);
	HAL_Delay(100);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command6,strlen(Command6),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command7,strlen(Command7),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command8,strlen(Command8),1000);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command9,strlen(Command9),1000);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command10,strlen(Command10),1000);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command11,strlen(Command11),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command12,strlen(Command12),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command13,strlen(Command13),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command14,strlen(Command14),100);
	HAL_Delay(250);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command15,strlen(Command15),100);
	HAL_Delay(1000);

	memset(RxBuffer,0,sizeof(char)*strlen(RxBuffer));
	HAL_UART_Transmit(&huart1,(uint8_t*)Command16,strlen(Command16),100);
	HAL_Delay(100);

}
/*
void reset_array()
{
	for(int i=0;i<25;i++)
	{
		RxBuffer[i]='0';
	}
}*/

uint8_t Check_OK()
{
	uint8_t ans=3;

	for(int i=0;i<strlen(RxBuffer);i++)
	{
		if(RxBuffer[i]=='O')//Check OK
		{

			if(RxBuffer[i+1]=='K')
			{
				ans=1;
			}

		}

		if(RxBuffer[i]=='E')//Check ERROR
		{
			if(RxBuffer[i+1]=='R')
			{
				if(RxBuffer[i+2]=='R')
				{
					if(RxBuffer[i+3]=='O')
					{
						if(RxBuffer[i+4]=='R')
						{
							ans=2;
						}
					}
				}
			}
		}

	}

	return ans;

}
