/*
 * GPS.c
 *
 *  Created on: Sep 4, 2023
 *      Author: CASPER
 */


#include <GPS.h>
#include <stdbool.h>
#include <SIM800.h>

extern char Buffer[500];
extern 	char Data[100];
extern UART_HandleTypeDef huart2;


// ------------------STRUCTURES----------------------

struct GGA_Str
{
	uint8_t hh;
	uint8_t mm;
	uint8_t ss;
	char pol1;
	float longitude;
	char pol2;
	float latitude;
};

struct Coordinate//(latitude,longitude)=(enlem,boylam)
{
	float latitude;
	char L_hemisphere;
	float longitude;
	char T_hemisphere;

};

// ------------------VARIABLES----------------------
char GPRMC_Data[100],GPGGA_Data[100];
bool GPS_Connection_Stat = false;//CHECK GPS CONNECTION
bool GPS_Location_Data_Received =false;

struct GGA_Str Time;
struct Coordinate Coord;

// -------------------------------------------------
uint16_t Strting_point;

// ------------------EXTERN----------------------
// ----------------------------------------------


// ------------------OTHERS---------------------

enum
{
	GPRMC,
	GPVTG,
	GPGGA,
	GPGSA,
	GPGSV,
	GPGLL
};

// ----------------------------------------------

void Set_Time()
{

	uint8_t Comma_Trig=0;
	uint16_t syc=0;
/*  //ESKİ ALGORİTMA ++ÇALIŞIYOR
	Time.hh = ((GPGGA_Data[7]-'0')*10+(uint8_t)GPGGA_Data[8]-'0')+3; //GMT +3
	Time.mm = (GPGGA_Data[9]-'0')*10+(uint8_t)GPGGA_Data[10]-'0';
	Time.ss = (GPGGA_Data[11]-'0')*10+(uint8_t)GPGGA_Data[12]-'0';
*/
	Get_GGA();

	while(Comma_Trig<=0)//GPGGA_Data bufferındaki verilerin analizi ',' e göre yapılmıştır
	{
		if(GPGGA_Data[syc]==',')
		{
			if(GPGGA_Data[syc+1] == ',')//No data has received yet
			{
				Time.hh=0;
				Time.mm=0;
				Time.ss=0;

			}

			else
			{
				Time.hh= ((GPGGA_Data[syc+1]-'0')*10+(uint8_t)GPGGA_Data[syc+2]-'0')+3;
				Time.mm = (GPGGA_Data[syc+3]-'0')*10+(uint8_t)GPGGA_Data[syc+4]-'0';
				Time.ss = (GPGGA_Data[syc+5]-'0')*10+(uint8_t)GPGGA_Data[syc+6]-'0';
			}

			Comma_Trig=1;
		}

		syc++;
	}

}
void Send_Time()
{
	SIM800l_Send_Data(Time.mm);
}
void Set_Location()
{

	uint8_t Comma_Trig=0;
	uint16_t syc=0;

	while(Comma_Trig<2)//GPGGA_Data bufferındaki verilerin analizi ',' e göre yapılmıştır
	{

		if(GPGGA_Data[syc]==',')
		{

			Comma_Trig++;
		}
			syc++;
	}


	if(Comma_Trig==2)
	{
		if(GPGGA_Data[syc]==',')//Any very hasn't received yet
		{
			GPS_Location_Data_Received = false;
		}
		else GPS_Location_Data_Received = true;
	}
//YENİ ALGORİTMA -- ',' e göre hesaplanıyor
/*
	Coord.latitude= (GPGGA_Data[syc]-'0')*10 + (GPGGA_Data[syc+1]-'0')*1 + (GPGGA_Data[syc+2]-'0')*0.1 + (GPGGA_Data[syc+3]-'0')*0.01 + (GPGGA_Data[syc+4]-'0')*0.001 + (GPGGA_Data[syc+5]-'0')*0.0001 + (GPGGA_Data[syc+6]-'0')*0.00001 + (GPGGA_Data[syc+7]-'0')*0.000001 + (GPGGA_Data[syc+8]-'0')*0.0000001 + (GPGGA_Data[syc+9]-'0')*0.00000001;
	Coord.L_hemisphere=GPGGA_Data[syc+11];
	Coord.longitude= (GPGGA_Data[syc+14]-'0')*10 + (GPGGA_Data[syc+15]-'0')*1 + (GPGGA_Data[syc+16]-'0')*0.1 + (GPGGA_Data[syc+17]-'0')*0.01 + (GPGGA_Data[syc+18]-'0')*0.001 + (GPGGA_Data[syc+19]-'0')*0.0001 + (GPGGA_Data[syc+20]-'0')*0.00001 + (GPGGA_Data[syc+21]-'0')*0.000001 + (GPGGA_Data[syc+22]-'0')*0.0000001 + (GPGGA_Data-'0')[syc+23]*0.00000001;
	Coord.T_hemisphere = GPGGA_Data[syc+25];
*/
//----------------------------------
	Coord.latitude= (GPGGA_Data[17]-'0')*10 + (GPGGA_Data[18]-'0')*1 + (GPGGA_Data[19]-'0')*0.1 + (GPGGA_Data[20]-'0')*0.01 + (GPGGA_Data[21]-'0')*0.001 + (GPGGA_Data[22]-'0')*0.0001 + (GPGGA_Data[23]-'0')*0.00001 + (GPGGA_Data[24]-'0')*0.000001 + (GPGGA_Data[25]-'0')*0.0000001 + (GPGGA_Data[26]-'0')*0.00000001;
	Coord.L_hemisphere=GPGGA_Data[28];
	Coord.longitude= (GPGGA_Data[31]-'0')*10 + (GPGGA_Data[32]-'0')*1 + (GPGGA_Data[33]-'0')*0.1 + (GPGGA_Data[34]-'0')*0.01 + (GPGGA_Data[35]-'0')*0.001 + (GPGGA_Data[36]-'0')*0.0001 + (GPGGA_Data[37]-'0')*0.00001 + (GPGGA_Data[38]-'0')*0.000001 + (GPGGA_Data[39]-'0')*0.0000001 + (GPGGA_Data-'0')[40]*0.00000001;
	Coord.T_hemisphere = GPGGA_Data[42];

	if(Coord.L_hemisphere == 'N' || Coord.L_hemisphere == 'S' && Coord.T_hemisphere == 'E'  || Coord.T_hemisphere == 'W')
	{
		GPS_Connection_Stat = true;
	}
	else GPS_Connection_Stat = false;
}
void Send_Location()
{
	//SIM800l_Send_Data(Coord.latitude,Coord.longitude);
	SIM800l_Send_Location(Coord.latitude,Coord.longitude);
}


void Get_RMC()//Recommended Minimum Navigation Information
{
	//HAL_UART_Receive(&huart2,(uint8_t*)Buffer,500, 1000);
	Reorder_data(Sp_finder(GPRMC),GPRMC);

}

void Get_GGA()//GLOBAL POSITIONING SYSTEM FIX DATA
{
	//HAL_UART_Receive(&huart2,(uint8_t*)Buffer,500, 1000);
	Reorder_data(Sp_finder(GPGGA),GPGGA);
}

uint16_t Sp_finder(uint8_t s_case)
{
	switch(s_case)
	{

		case 0:  //GPRMC
			for(int i=0;i<500;i++)
			{
				if(Buffer[i]== '$')
				{
					if(Buffer[i+1]== 'G')
					{
						if(Buffer[i+2]== 'P')
						{
							if(Buffer[i+3]== 'R')
							{
								if(Buffer[i+4]== 'M')
								{
									if(Buffer[i+5]== 'C')
									{
										Strting_point=i;
										return (uint16_t)i;
										//Reorder_data(i);
									}
								}
							}
						}
					}
				}
			}
			break;

		case 1:

			return;
			break;

		case 2://GPGGA
			for(int i=0;i<500;i++)
			{
				if(Buffer[i]== '$')
				{
					if(Buffer[i+1]== 'G')
					{
						if(Buffer[i+2]== 'P')
						{
							if(Buffer[i+3]== 'G')
							{
								if(Buffer[i+4]== 'G')
								{
									if(Buffer[i+5]== 'A')
									{
										Strting_point=i;
										return (uint16_t)i;
										//Reorder_data(i);
									}
								}
							}
						}
					}
				}
			}
			return;
			break;


		case 3:

			return;
			break;
		case 4:

			return;
			break;
		case 5:

			return;
			break;

		default:
			return;
			break;

	}

}

void Reorder_data(uint16_t sp,uint8_t s_case)
{
	uint16_t syc=0;
	while(Buffer[sp]!='\r'&& Buffer[sp+1]!='\n')
	{

		syc++;
		sp++;

		switch(s_case)
		{

			case 0:
				GPRMC_Data[syc]=Buffer[sp];
				break;

			case 1:


				break;

			case 2:
				GPGGA_Data[syc]=Buffer[sp];

				break;


			case 3:


				break;
			case 4:


				break;
			case 5:


				break;

			default:

				break;

		}

	}
}
