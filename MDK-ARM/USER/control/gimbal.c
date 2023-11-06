/*
* gimbal
*	
*	
* ��̨
*/


/* Includes ------------------------------------------------------------------*/
#include "gimbal.h"
#include "device.h"
#include "rp_math.h"
#include "car.h"
/* Private function prototypes -----------------------------------------------*/

void Gimbal_Can_Update(gimbal_t *gimbal);								//��̨ yaw pitch can ����

void Gimbal_Gyro_Update(gimbal_t *gimbal,uint8_t ctrl_mode);      //������ģʽң��������
void Gimbal_Mec_Update(gimbal_t *gimbal,uint8_t ctrl_mode);       //��еģʽң��������

void Gimbal_Pitch_Gyro_Angle_Limit(gimbal_t *gimbal);						//��̨pitch�������ǽǶ���λ
void Gimbal_Pitch_Mec_Angle_Limit(gimbal_t *gimbal);					//��̨pitch���е�Ƕ���λ
void Gimbal_Yaw_Angle_Check(gimbal_t *gimbal);              //��̨yawĿ��Ƕȼ��

void Gimbal_Gyro_Pid_Calculating(gimbal_t *gimbal);					//������ģʽ PID Calculating
void Gimbal_Mec_Pid_Calculating(gimbal_t *gimbal);          //��еģʽ PID Calculating 
	
void Gimbal_Work_Gyro(gimbal_t *gimbal);											//������ģʽ
/* Exported variables --------------------------------------------------------*/

gimbal_t gimbal = 
{
	.gimbal_y = &gimbal_motor[GIMBAL_YAW],
	.gimbal_p = &gimbal_motor[GIMBAL_PITCH],

	
	.work = Gimbal_Work,
	
	.gimbal_reset_state = DEV_RESET_NO,
};
/* Function  body --------------------------------------------------------*/





/**
  * @brief  ��̨���������ݸ���
  * @param  
  * @retval 
  */
void Gimbal_Imu_Update(gimbal_t *gimbal)
{

	gimbal->base_info.yaw_imu_angle = -imu_sensor.info->base_info.yaw;
//	vector_update(&vector_yaw_imu_angle, info->yaw_imu_angle);
	gimbal->base_info.yaw_imu_speed = -imu_sensor.info->base_info.rate_yaw;
	gimbal->base_info.pitch_imu_angle = -imu_sensor.info->base_info.pitch;
	gimbal->base_info.pitch_imu_speed = -imu_sensor.info->base_info.rate_pitch;
  
//	vector_update(&vector_yaw_imu_speed, info->yaw_imu_speed);
}

/**
  * @brief  ��̨pitch�������ǽǶ���λ
  * @param  
  * @retval 
  */
void Gimbal_Pitch_Gyro_Angle_Limit(gimbal_t *gimbal)
{

	float angle = gimbal->base_info.pitch_imu_angle_target;
	if(angle > GIMBAL_MAX_GYRO_ANGEL)
	{
		angle = GIMBAL_MAX_GYRO_ANGEL;
	}
	if(angle < GIMBAL_MIN_GYRO_ANGEL)
	{
		angle = GIMBAL_MIN_GYRO_ANGEL;
	}
	gimbal->base_info.pitch_imu_angle_target = angle;
}

/**
  * @brief  ��̨pitch���е�Ƕ���λ
  * @param  
  * @retval 
  */
void Gimbal_Pitch_Mec_Angle_Limit(gimbal_t *gimbal)
{

	float angle = gimbal->base_info.pitch_mec_angle_target;
	if(angle > GIMBAL_MAX_MEC_ANGEL)
	{
		angle = GIMBAL_MAX_MEC_ANGEL;
	}
	if(angle < GIMBAL_MIN_MEC_ANGEL)
	{
		angle = GIMBAL_MIN_MEC_ANGEL;
	}
	gimbal->base_info.pitch_mec_angle_target = angle;
}



/**
  * @brief  ��̨������yawĿ��Ƕȼ�� ������Ŀ��Ƕ����ۼӵ���Ҫ����
  * @param  
  * @retval 
  */
void Gimbal_Yaw_Angle_Check(gimbal_t *gimbal)
{
	float angle = gimbal->base_info.yaw_imu_angle_target;//-180��~180��
	while (abs(angle) > 180)
	{
		angle -= 360 * sgn(angle);
	}
	gimbal->base_info.yaw_imu_angle_target = angle;
	
}
/**
  * @brief  ��̨ yaw pitch can ����
  * @param  
  * @retval 
  */
void Gimbal_Can_Update(gimbal_t *gimbal)
{
	//����㴦��
	if(gimbal->gimbal_y->rx_info.angle > (YAW_MOTOR_ANGLE_MIDDLE + 4096)) 
	{
		gimbal->base_info.yaw_motor_angle =  8192 + YAW_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_y->rx_info.angle;
	}
	else
	{
		gimbal->base_info.yaw_motor_angle =  YAW_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_y->rx_info.angle;
	}
	
	gimbal->base_info.yaw_motor_speed = - gimbal->gimbal_y->rx_info.speed;  
	
	gimbal->base_info.pitch_motor_angle =  PITCH_MOTOR_ANGLE_MIDDLE - gimbal->gimbal_p->rx_info.angle;
	gimbal->base_info.pitch_motor_speed =  - gimbal->gimbal_p->rx_info.speed;  
	

}

/**
 * @brief ������ģʽң��������
 * 
 * @param gimbal 
 * @param ctrl_mode 0:ң����ģʽ 1������ģʽ
 */
void Gimbal_Gyro_Update(gimbal_t *gimbal,uint8_t ctrl_mode)
{
	if(ctrl_mode == 0)//ң����ģʽ
	{

		gimbal->base_info.yaw_imu_angle_target +=  rc.base_info->ch0 / 2000.f; //�������330dps
		gimbal->base_info.pitch_imu_angle_target  +=  rc.base_info->ch1 / 10000.f; 
	}
	else if(ctrl_mode == 1)//����ģʽ
	{
		gimbal->base_info.yaw_imu_angle_target +=  rc.info->mouse_x_K / 500.f; 
		gimbal->base_info.pitch_imu_angle_target  -= rc.info->mouse_y_K / 1000.f; 

	}
	Gimbal_Yaw_Angle_Check(gimbal);
	Gimbal_Pitch_Gyro_Angle_Limit(gimbal);
	
	//��еpitchʵʱ����
	gimbal->base_info.pitch_mec_angle_target = gimbal->base_info.pitch_motor_angle;
	Gimbal_Pitch_Mec_Angle_Limit(gimbal);
}

/**
  * @Name    
  * @brief  ��еģʽ���� 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Mec_Update(gimbal_t *gimbal,uint8_t ctrl_mode)
{
	gimbal->base_info.yaw_mec_angle_target = 0 ;
	if (ctrl_mode == 0)
	{
		gimbal->base_info.pitch_mec_angle_target  +=  rc.base_info->ch1 / 500.f; 
	}
	else if(ctrl_mode == 1)
	{
		gimbal->base_info.pitch_mec_angle_target  -= rc.info->mouse_y_K / 3000.f; 
	}
	 
	
	
	gimbal->base_info.yaw_imu_angle_target = gimbal->base_info.yaw_imu_angle;
	Gimbal_Pitch_Mec_Angle_Limit(gimbal);
	
	gimbal->base_info.pitch_imu_angle_target = gimbal->base_info.pitch_imu_angle;
	gimbal->base_info.yaw_imu_angle_target = gimbal->base_info.yaw_imu_angle;
	Gimbal_Pitch_Gyro_Angle_Limit(gimbal);
}
/**
  * @Name    
  * @brief ������ģʽ PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Gyro_Pid_Calculating(gimbal_t *gimbal)
{
	//yaw pid
	float yaw_meas_out = gimbal->base_info.yaw_imu_angle;				//�⻷
	float yaw_meas_in = gimbal->base_info.yaw_imu_speed	;			//�ڻ�
	float yaw_target = gimbal->base_info.yaw_imu_angle_target;
	
	gimbal->base_info.output_gimbal_y = -gimbal->gimbal_y->c_pid2( &gimbal->gimbal_y->pid.gyro_out ,  \
																																&gimbal->gimbal_y->pid.gyro_in  ,	 \
																																yaw_meas_out ,yaw_meas_in ,yaw_target ,3);
	
	//pitch pid
	float pitch_meas_out = gimbal->base_info.pitch_imu_angle;				//�⻷
	float pitch_meas_in = gimbal->base_info.pitch_imu_speed	;			//�ڻ�
	float pitch_target = gimbal->base_info.pitch_imu_angle_target;
	
	gimbal->base_info.output_gimbal_p = -gimbal->gimbal_p->c_pid2( &gimbal->gimbal_p->pid.gyro_out,	\
																																&gimbal->gimbal_p->pid.gyro_in,		\
																																pitch_meas_out,pitch_meas_in,pitch_target,1);
}

/**
  * @Name    
  * @brief ��еģʽ PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Mec_Pid_Calculating(gimbal_t *gimbal)
{
	//yaw pid
	float yaw_meas_out = gimbal->base_info.yaw_motor_angle / 4096.f * 180.f;	 //�⻷ תΪ�Ƕ�
	float yaw_meas_in = gimbal->base_info.yaw_motor_speed	;			//�ڻ�
	float yaw_target = gimbal->base_info.yaw_mec_angle_target;
	
	gimbal->base_info.output_gimbal_y = -gimbal->gimbal_y->c_pid2( &gimbal->gimbal_y->pid.mec_out ,  \
																																&gimbal->gimbal_y->pid.mec_in  ,	 \
																																yaw_meas_out ,yaw_meas_in ,yaw_target ,3);
	
	//pitch pid
	float pitch_meas_out = gimbal->base_info.pitch_motor_angle ;				//�⻷
	float pitch_meas_in = gimbal->base_info.pitch_motor_speed	;			//�ڻ�
	float pitch_target = gimbal->base_info.pitch_mec_angle_target;
	
	gimbal->base_info.output_gimbal_p = -gimbal->gimbal_p->c_pid2( &gimbal->gimbal_p->pid.mec_out,	\
																																&gimbal->gimbal_p->pid.mec_in,		\
																																pitch_meas_out,pitch_meas_in,pitch_target,1);
}
/**
  * @Name    
  * @brief  ������ģʽ 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Work_Gyro(gimbal_t *gimbal)
{
	if(car.gim_180_run_state != CMD_RUNING)//��ͷʱң�������ܸı�Ŀ��ֵ
	{
		Gimbal_Gyro_Update(gimbal,car.car_ctrl_mode);
	}
	Gimbal_Gyro_Pid_Calculating(gimbal);
	
	//������
	gimbal->gimbal_p->base_info.motor_out = gimbal->base_info.output_gimbal_p;
	gimbal->gimbal_y->base_info.motor_out = gimbal->base_info.output_gimbal_y;
}

/**
  * @Name    
  * @brief  ��еģʽ 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Gimbal_Work_Mec(gimbal_t *gimbal)
{
	Gimbal_Mec_Update(gimbal,car.car_ctrl_mode);
	Gimbal_Mec_Pid_Calculating(gimbal);
	
	//������
	gimbal->gimbal_p->base_info.motor_out = gimbal->base_info.output_gimbal_p;
	gimbal->gimbal_y->base_info.motor_out = gimbal->base_info.output_gimbal_y;
}

/**
  * @Name    Gimbal_Work
  * @brief   �ܿ�
  * @param   
  * @retval 
  * @author  
  * @Date    
**/

void Gimbal_Work(gimbal_t *gimbal)
{
	Gimbal_Can_Update(gimbal);
	
	//����ִ��
	if(CAR_R90_CMD  ==  true && car.car_r90_run_flag == CMD_RUN_OK) 
	{
		gimbal->base_info.yaw_imu_angle_target += 90;
		car.car_r90_run_flag = CMD_RUN_NO; //ִ����ɣ��ȴ�������λ
		
	}
	if(CAR_L90_CMD  ==  true && car.car_l90_run_flag == CMD_RUN_OK) 
	{
		gimbal->base_info.yaw_imu_angle_target -= 90;
		car.car_l90_run_flag = CMD_RUN_NO; //ִ����ɣ��ȴ�������λ
	}

  //��̨ת180
	int16_t *angel_last = &gimbal->base_info.cmd_yaw_angle_last;
	if(GIM_180_CMD  ==  true )
	{
		if(car.gim_180_run_flag == CMD_RUN_OK)
		{		
			car.gim_180_run_state = CMD_RUNING;
			*angel_last = gimbal->base_info.yaw_imu_angle;
			gimbal->base_info.yaw_imu_angle_target = *angel_last - sgn(*angel_last)*180;
			car.gim_180_run_flag = CMD_RUN_NO;
		}
	}
  if(abs(gimbal->base_info.yaw_imu_angle- (*angel_last - sgn(*angel_last)*180)) < 1)
 	{
  	car.gim_180_run_state = CMD_FINISH;
 	}
	
	//ģʽ�ж�
	switch(car.car_move_mode)
	{
		//����
		case(offline_CAR):
			gimbal->base_info.output_gimbal_y = 0;
			gimbal->base_info.output_gimbal_p = 0;
			gimbal->gimbal_reset_state = DEV_RESET_NO;
			break;
		//��ʼ��
		case(init_CAR):
			gimbal->base_info.pitch_mec_angle_target = 0;
      gimbal->base_info.yaw_mec_angle_target = 0;
			//�жϳ�ʼ���Ƿ����
			if(abs(gimbal->base_info.yaw_motor_angle) <= 13 && abs(gimbal->base_info.pitch_motor_angle) <= 40   \
      && abs(gimbal->base_info.pitch_motor_speed) <= 3 && abs(gimbal->base_info.pitch_motor_speed) <= 3 )
			{
				gimbal->gimbal_reset_state = DEV_RESET_OK;
			}
			else
			{
				//��еģʽ���г�ʼ��
        Gimbal_Work_Mec(gimbal);
			}
		break;
		//��еģʽ
		case(mec_CAR):
			Gimbal_Work_Mec(gimbal);
			break;
		//������ģʽ
		case(gyro_CAR):
			Gimbal_Work_Gyro(gimbal);
			break;
		//С����ģʽ
		case(cycle_CAR):
			Gimbal_Work_Gyro(gimbal);
			break;
		default:
			break;
			
	}

}







