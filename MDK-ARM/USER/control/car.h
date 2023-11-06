#ifndef __CAR_H
#define __CAR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "remote.h"
#include "config.h"

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* ���ж�ģʽö�� */
typedef enum 
{
  offline_CAR,        //����ģʽ     0
  init_CAR,           //��ʼ��ģʽ   1
  mec_CAR,            //��еģʽ     2
  gyro_CAR,           //������ģʽ   3
	cycle_CAR,					//С����ģʽ   4

}Car_Move_Mode_e;

/* ������ģʽö�� */
typedef enum 
{
  RC_CTRL_MODE,        //ң����ģʽ  0
  KEY_CTRL_MODE,       //����ģʽ    1

}Car_Ctrl_Mode_e;

/* ִ������Flag  ������λ->OK ִ����->NO*/
typedef enum 
{
	CMD_RUN_NO, //�����ִ��
	CMD_RUN_OK, //��ִ������
	
}Cmd_Run_Flag_e;

/* ����ִ��״̬ */
typedef enum 
{
	CMD_FINISH, //ִ�����
	CMD_RUNING, //ִ����
	
}Cmd_Run_State_e;

/**
 * @brief car��ṹ��
 * 
 */
typedef struct
{
	Car_Move_Mode_e car_move_mode;


	Dev_Reset_State_e	 car_reset_state;
	Car_Ctrl_Mode_e  car_ctrl_mode;

	Cmd_Run_Flag_e car_l90_run_flag;
	Cmd_Run_Flag_e car_r90_run_flag;
	Cmd_Run_Flag_e gim_180_run_flag;
	Cmd_Run_Flag_e shooting_fire_run_flag;

	Cmd_Run_State_e gim_180_run_state;
	Cmd_Run_State_e shooting_fire_run_state;
	Cmd_Run_State_e shooting_firing_run_state;
}car_t ;

extern car_t car;

extern bool CAR_R90_CMD;      //��ת90���� 
extern bool CAR_L90_CMD;      //��ת90���� 
extern bool GIM_180_CMD;      //��̨��ͷ����
extern bool SHOOTING_FIRE_CMD; 		//���������
extern bool SHOOTING_FIRING_CMD; //������������
/* Exported functions --------------------------------------------------------*/
void Car_Ctrl(car_t *car) ;

#endif
