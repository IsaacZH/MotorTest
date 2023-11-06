#ifndef _GIMBAL_H
#define _GIMBAL_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "gimbal_motor.h"

/* Private macro -------------------------------------------------------------*/
#define YAW_MOTOR_ANGLE_MIDDLE 1457  		//YAW�����ֵ 0x05B1
#define PITCH_MOTOR_ANGLE_MIDDLE 4604		//pitch�����ֵ 0x11FC
#define GIMBAL_MAX_GYRO_ANGEL		(30.f)	//pitch�����ǽǶȵ����λ���ֵ
#define GIMBAL_MIN_GYRO_ANGEL		(-30.f)	//pitch�����ǽǶȵ����λ��Сֵ

#define GIMBAL_MAX_MEC_ANGEL   (740.f)				//pitchJ��е�Ƕȵ����λ���ֵ
#define GIMBAL_MIN_MEC_ANGEL   (-511.f)			//pitch��е�Ƕȵ����λ��Сֵ

/* Private typedef -----------------------------------------------------------*/

/** 
  * @brief  ��̨������Ϣ����
  */ 
typedef struct __packed 
{
	float yaw_imu_angle;										//��̨������yaw��Ƕ�
	float yaw_imu_speed;										//��̨������yaw���ٶ�
	float pitch_imu_angle;									//��̨������pitch��Ƕ�
	float pitch_imu_speed;                  //��̨������pitch���ٶ�
	
	float  yaw_imu_angle_target;         //������ģʽĿ��yaw   ��������ϵ (-180��~180��) (˳ʱ��Ϊ��)
	float  pitch_imu_angle_target;      //������ģʽĿ��pitch  ��������ϵ (-90��~90��)   (����Ϊ��)
	
	int16_t  yaw_motor_angle;           //yaw�� ��Ե���  �Ƕ�(-4096~4096)      (˳ʱ��Ϊ��)
	int16_t  yaw_motor_speed;           //yaw�� ��Ե���  �ٶ�(rpm)             (˳ʱ��Ϊ��)
	
	int16_t  pitch_motor_angle;           //pitch�� ��Ե���   �Ƕ�(-2048~2048)    (����Ϊ��)
	int16_t  pitch_motor_speed;           //pitch�� ��Ե���   �ٶ�(rpm)           (����Ϊ��)
	

	float  yaw_mec_angle_target;				//��еģʽĿ��yaw		��������ϵ(-180��~180��)
	float  pitch_mec_angle_target;			//��еģʽĿ��pitch	��������ϵ	(-2048~2048)  (����Ϊ��)
	
	int16_t  output_gimbal_y;							//yaw�������
	int16_t  output_gimbal_p;							//pitch�������

	int16_t  cmd_yaw_angle_last;   //�洢��ͷǰyaw�Ƕ�
}gimbal_base_info_t;


/** 
  * @brief  ��̨�ඨ��
  */ 
typedef struct gimbal_class_t
{	

	motor_t        		 *gimbal_p;
	motor_t        		 *gimbal_y;

	
	gimbal_base_info_t   	base_info;
//	motor_state_e   		work_sate;
	
	void                (*work)(struct gimbal_class_t *gimbal);
	Dev_Reset_State_e			 gimbal_reset_state;
	
}gimbal_t;


extern gimbal_t gimbal;



/* Private variables ---------------------------------------------------------*/


/* Exported variables ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

void Gimbal_Work(gimbal_t *gimbal);													//�ܿ�
void Gimbal_Imu_Update(gimbal_t *gimbal);										//��̨���������ݸ���
#endif
