/*
* chassis
*	
*	2023.9.11
* ����
*/

/* Includes ------------------------------*/
#include "chassis.h"
#include "gimbal.h"
#include "device.h"
#include "rp_math.h"
#include "car.h"

/* Private function prototypes -----------------------------------------------*/

void Chassis_Speed_Calculating(chassis_t *chassis);//���ֵ��̵��Ŀ���ٶȽ���
void Chassis_Pid_Calculating(chassis_t *chassis);//����PID����
void Chassis_Speed_Update(chassis_t *chassis);//������̵�ǰ�ٶ�

void Chassis_Gyro_Update(chassis_t *chassis,uint8_t ctrl_mode, uint8_t move_mode);//����������ģʽ����
void Chassis_Mec_Update(chassis_t *chassis,uint8_t ctrl_mode);//���̻�еģʽ���� 

void Chassis_Work_Mec(chassis_t *chassis);//��еģʽ
void Chassis_Work_Gyro(chassis_t *chassis);//������ģʽ

/* Exported variables --------------------*/
chassis_t chassis = 
{
	.chassisLF = &chassis_motor[CHASSIS_LF],
	.chassisLB = &chassis_motor[CHASSIS_LB],
	.chassisRF = &chassis_motor[CHASSIS_RF],
	.chassisRB = &chassis_motor[CHASSIS_RB],
	
	.work = Chassis_Work,
	
};

/* Function  body --------------------------------------------------------*/



/**
  * @brief  ���̵�ǰ�ٶ��ٶȸ���
  * @param  
  * @retval 
**/
void Chassis_Speed_Update(chassis_t *chassis)
{
	//��ȡ��ǰ����ٶ�
	int16_t LF_speed = chassis->chassisLF->rx_info.speed;
	int16_t LB_speed = chassis->chassisLB->rx_info.speed;
	int16_t RF_speed = chassis->chassisRF->rx_info.speed;
	int16_t RB_speed = chassis->chassisRB->rx_info.speed;
	

	
	//�����ٶ�
	chassis->base_info.Front_speed = (LF_speed - RF_speed + LB_speed - RB_speed) / 4;
	chassis->base_info.Right_speed = (LF_speed + RF_speed - LB_speed - RB_speed) / 4;
	chassis->base_info.Cycle_speed = (LF_speed + RF_speed + LB_speed + RB_speed) / 4;
	

}

/**
 * @brief ���̻�еģʽ����
 * 
 * @param chassis 
 * @param ctrl_move 0:ң����ģʽ 1������ģʽ
 */
void Chassis_Mec_Update(chassis_t *chassis,uint8_t ctrl_mode)
{
	if(ctrl_mode == 0)//ң����ģʽ
	{
		chassis->base_info.target_front_speed = (float) rc.base_info->ch3 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed = (float) rc.base_info->ch2 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_cycle_speed = (float) rc.base_info->ch0 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
	}
	else if (ctrl_mode == 1)//����ģʽ
	{
		chassis->base_info.target_front_speed = 0;
		chassis->base_info.target_right_speed = 0;
		chassis->base_info.target_front_speed += (float)rc.base_info->W.cnt / (float)KEY_W_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_front_speed -= (float)rc.base_info->S.cnt / (float)KEY_S_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed += (float)rc.base_info->D.cnt / (float)KEY_D_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_right_speed -= (float)rc.base_info->A.cnt / (float)KEY_A_CNT_MAX * CHASSIS_MAX_SPEED;
		chassis->base_info.target_cycle_speed =  rc.info->mouse_x_K;
	}
}

/**
 * @brief ����������ģʽ����
 * 
 * @param chassis 
 * @param ctrl_move 0:ң����ģʽ 1������ģʽ
 * @param move_mode 0:������ģʽ 1��С����ģʽ
 * @note ͨ�������̻�еģʽ���¡�����Ŀ��ֵ���ٰ�Ŀ��ֵӳ�䵽����
 */
void Chassis_Gyro_Update(chassis_t *chassis,uint8_t ctrl_mode, uint8_t move_mode)
{
	Chassis_Mec_Update(chassis,ctrl_mode);
	float front = chassis->base_info.target_front_speed;
	float right = chassis->base_info.target_right_speed;

	//��������������͵�������ǶȲ�
	int16_t yaw_angle_err;
	float yaw_angle_err_rad; 
	yaw_angle_err  = gimbal.base_info.yaw_motor_angle;							//yaw��   ��Ե���   �Ƕ�(-4096~4096)(˳ʱ��Ϊ��)
	yaw_angle_err_rad = (double)yaw_angle_err / 4096.f * 3.14159;   //yaw��Ƕ�ת�����ƣ�һȦ2�У�

	//front��rightֵ����
	chassis->base_info.target_front_speed =  front * cos(yaw_angle_err_rad) - right * sin(yaw_angle_err_rad);
	chassis->base_info.target_right_speed =  right * cos(yaw_angle_err_rad) + front * sin(yaw_angle_err_rad);


	if(move_mode == 0)
	{
		//cycleֵ����
		if(abs(yaw_angle_err) > 2048)//ͷ���Գ���
		{
			yaw_angle_err -= 4096 * sgn(yaw_angle_err);
		}
		if(abs(yaw_angle_err) >= 100) //����4.4��б�ʱ�� 
		{
			chassis->base_info.target_cycle_speed = yaw_angle_err * 6.f - 300.f * sgn(yaw_angle_err);
		}
		else 
		{
			chassis->base_info.target_cycle_speed = yaw_angle_err * 3.f;
		}
	}
	else if (move_mode == 1)
	{
		chassis->base_info.target_cycle_speed = 3000;//С�����ٶ�
	}
	
}


/**
  * @Name    Chassis_Speed_Calculating
  * @brief   ���̵��Ŀ���ٶȼ���
  * @param   
  * @retval
  * @author  HWX CCB Isaac 
  * @Date    2022-12-3
**/
void Chassis_Speed_Calculating(chassis_t *chassis)
{
	int16_t front = chassis->base_info.target_front_speed;
	int16_t right = chassis->base_info.target_right_speed;
	int16_t cycle = chassis->base_info.target_cycle_speed;
	
		
	int16_t speed_sum;
	float K;
	
	speed_sum = abs(front) + abs(right) + abs(cycle);
	
	if(speed_sum > CHASSIS_MAX_SPEED)
	{
		K = (float)CHASSIS_MAX_SPEED / (float)speed_sum;
	}
	else 
	{
		K = 1;
	}

	front *= K;
	right *= K;
	cycle *= K;
	
	chassis->base_info.target_chassisLF   =   front + right + cycle; 
	chassis->base_info.target_chassisLB   =   front - right + cycle;
	chassis->base_info.target_chassisRF   = - front + right + cycle; 
	chassis->base_info.target_chassisRB   = - front - right + cycle; 
	
}

/**
  * @Name    
  * @brief  PID Calculating 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Pid_Calculating(chassis_t *chassis)
{

	chassis->base_info.output_chassisLF 	= chassis->chassisLF->c_speed(chassis->chassisLF, chassis->base_info.target_chassisLF);
	chassis->base_info.output_chassisLB 		= chassis->chassisLB->c_speed(chassis->chassisLB, chassis->base_info.target_chassisLB);	
	chassis->base_info.output_chassisRF 	= chassis->chassisRF->c_speed(chassis->chassisRF, chassis->base_info.target_chassisRF);
	chassis->base_info.output_chassisRB 		= chassis->chassisRB->c_speed(chassis->chassisRB, chassis->base_info.target_chassisRB);
}

/**
  * @Name    
  * @brief   ���̻�еģʽ
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Mec(chassis_t *chassis)
{
	Chassis_Speed_Update(chassis);
	Chassis_Mec_Update(chassis,car.car_ctrl_mode);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}

/**
  * @Name    
  * @brief   ����������ģʽ
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Gyro(chassis_t *chassis)
{
	
	Chassis_Speed_Update(chassis);
	Chassis_Gyro_Update(chassis,car.car_ctrl_mode,0);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}	

/**
  * @Name    
  * @brief   ����С����ģʽ
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Cycle(chassis_t *chassis)
{
	
	Chassis_Speed_Update(chassis);
	Chassis_Gyro_Update(chassis,car.car_ctrl_mode,1);
	Chassis_Speed_Calculating(chassis);
	Chassis_Pid_Calculating(chassis);
	
	chassis->chassisLF->base_info.motor_out 	= chassis->base_info.output_chassisLF;
	chassis->chassisLB->base_info.motor_out 	= chassis->base_info.output_chassisLB;	
	chassis->chassisRF->base_info.motor_out 	= chassis->base_info.output_chassisRF;
	chassis->chassisRB->base_info.motor_out 	= chassis->base_info.output_chassisRB;

}	

/**
  * @Name    Chassis_Work
  * @brief   �ܿ�
  * @param   
  * @retval 
  * @author  
  * @Date    
**/

void Chassis_Work(chassis_t *chassis)
{

  
  switch(car.car_move_mode)
	{

		case(offline_CAR):	
			chassis->base_info.output_chassisLF = 0;
			chassis->base_info.output_chassisLB = 0;
			chassis->base_info.output_chassisRF = 0;
			chassis->base_info.output_chassisRB = 0;
			break;
		case(init_CAR):
      chassis->base_info.target_cycle_speed = 0;
      chassis->base_info.target_front_speed = 0;
      chassis->base_info.target_right_speed = 0;
		break;
		case(mec_CAR):
			Chassis_Work_Mec(chassis);
			break;
		case(gyro_CAR):
			//������Ӧ
  			if(car.gim_180_run_state == CMD_RUNING)
  			{
  			  Chassis_Work_Mec(chassis);
  			}
				else
				{
					Chassis_Work_Gyro(chassis);
				}
			break;
		case(cycle_CAR):
			Chassis_Work_Cycle(chassis);
			break;
		default:
			break;
	}


}

