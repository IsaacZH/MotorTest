/**
 * @file car.c
 * @author Isaac
 * @brief ������־λ�ĸ���
 * @version 0.1
 * @date 2023-10-25
 * 
 * @copyright Isaac (c) 2023
 * @note �󲦸��½������ģʽ��ң����ģʽ�µ����Ҫ�������ͻ��
 * �״γ�ʼ����ɻ��Ƚ���������ģʽ
 */

/* Includes ------------------------------------------------------------------*/
#include "car.h"
#include "gimbal.h"

/* Private function prototypes -----------------------------------------------*/
void Car_Commond_Init(car_t *car);
void Commond_Update(car_t *car);
void Car_Mode_Update(car_t *car);
void RC_S1_Status_Check(car_t *car);
void RC_S2_Status_Check(car_t *car);
void Car_Init_Judge(car_t *car);
void RC_Wheel_Status_Check(car_t *car);
void	Car_Ctrl_Mode_Update(car_t *car);
void KEY_Shift_Status_Check(car_t *car);
void KEY_Ctrl_Status_Check(car_t *car);
void KEY_F_Status_Check(car_t *car);
void KEY_E_Status_Check(car_t *car);
void KEY_Q_Status_Check(car_t *car);
void KEY_V_Status_Check(car_t *car);
void KEY_ML_Status_Check(car_t *car);
/* Private variables ---------------------------------------------------------*/
car_t car=
{
	.car_reset_state = DEV_RESET_NO,

};


//����
bool CAR_L90_CMD   		  =  false;  //��ת90���� 
bool GIM_180_CMD   		  =  false;  //��̨��ͷ����
bool CAR_R90_CMD  		  =  false;  //��ת90���� 
bool SHOOTING_FIRE_CMD  =  false;  //���������
bool SHOOTING_FIRING_CMD = false;  //������������

/**
  * @Name    
  * @brief  �����ʼ��
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Car_Commond_Init(car_t *car)
{
	bool CAR_R90_CMD  =  false;  //��ת90���� 
	bool CAR_L90_CMD  =  false;  //��ת90���� 
	bool GIM_180_CMD  =  false;  //��̨��ͷ����
	bool SHOOTING_FIRE_CMD  =  false;  //���������
	bool SHOOTING_FIRING_CMD = false;  //������������
	//��ִ�г�ʼ��
	car->car_l90_run_flag = CMD_RUN_NO ;
	car->car_r90_run_flag = CMD_RUN_NO ;
	car->gim_180_run_flag = CMD_RUN_NO ;
	car->shooting_fire_run_flag = CMD_RUN_NO ; 
	
	car->gim_180_run_state = CMD_FINISH ;
	car->shooting_firing_run_state = CMD_FINISH;
	car->shooting_fire_run_state = CMD_FINISH;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @name     
  * @brief  ��������
  * @param   
  * @retval 
  * @author  
  * @date
**/
void Car_Ctrl(car_t *car) 
{
	/*����ģʽ����*/
	Car_Ctrl_Mode_Update(car);
	
	/* ����ģʽ���� */
  Car_Mode_Update(car);
	
	/* ����������� */
  Commond_Update(car);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @Name    
  * @brief  ��ʼ���ж� 
  * @param   
  * @retval 
  * @author      
**/
void Car_Init_Judge(car_t *car)
{

	if(gimbal.gimbal_reset_state == DEV_RESET_OK)
	{
		car->car_reset_state = DEV_RESET_OK;
	}
}
/**
  * @Name    
  * @brief  ����������� 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Commond_Update(car_t *car)
{
	if(car->car_ctrl_mode == RC_CTRL_MODE)
	{
		RC_Wheel_Status_Check(car);
		RC_S1_Status_Check(car);
	}
	else if (car->car_ctrl_mode == KEY_CTRL_MODE)
	{
		KEY_E_Status_Check(car);
		KEY_Q_Status_Check(car);
		KEY_V_Status_Check(car);
		KEY_ML_Status_Check(car);
	}
}

/**
  * @Name    
  * @brief  ����ģʽ���� 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Car_Mode_Update(car_t *car)
{
	Car_Init_Judge(car);
	
	if(!RC_ONLINE)
	{
		car->car_move_mode = offline_CAR;
		car->car_reset_state = DEV_RESET_NO;
		Car_Commond_Init(car);
	}
	else if(car->car_reset_state == DEV_RESET_NO)
	{
		car->car_move_mode = init_CAR;
	}
	else if (car->car_move_mode == init_CAR)
	{
		car->car_move_mode = gyro_CAR;//����Ĭ��ģʽ��������ģʽ��
	}
	else 
	{
		if (car->car_ctrl_mode == RC_CTRL_MODE)//ң����ģʽ
		{
			RC_S2_Status_Check(car);//�Ҳ��˿����ƶ�ģʽ
		}
		else//����ģʽ 
		{
			KEY_Shift_Status_Check(car);//�˳�������������ģʽ
			KEY_Ctrl_Status_Check(car);//��еģʽ�л�
			KEY_F_Status_Check(car);//С����ģʽ�л�
		}
		
	}
}

/**
 * @brief ����ģʽ����
 * 
 * @param car 
 */
void	Car_Ctrl_Mode_Update(car_t *car)
{
	//�󲦸��£�����ģʽ  ���඼��ң����ģʽ
	switch(rc.base_info->s1.value)
	{
	case 0x02:	//�󲦸���
    car->car_ctrl_mode = KEY_CTRL_MODE;
		break;
	default://�󲦸˲�����
		car->car_ctrl_mode = RC_CTRL_MODE;
		break;
	}

//��ң����ģʽת������ģʽ������������ģʽ
	switch (rc.base_info->s1.status)
	{
	case down_R://�󲦸����²�
		car->car_move_mode = gyro_CAR;
		break;
	
	default:
		break;
	}
}


/**
  * @Name    
  * @brief  ң�����Ҳ���״̬���
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_S2_Status_Check(car_t *car)
{
	switch(rc.base_info->s2.value)
	{
	case 0x01:  //�Ҳ�����
		car->car_move_mode  =   mec_CAR ;
		break;
	case 0x03:	//�Ҳ�����
		car->car_move_mode  =   gyro_CAR;
		break;
	case 0x02:	//�Ҳ�����
		car->car_move_mode  =   cycle_CAR;
		break;
	default:
		break;		
	}
}
/**
  * @Name    
  * @brief  ң��������״̬���
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_Wheel_Status_Check(car_t *car)
{
	switch(rc.base_info->thumbwheel.status)
	{
	case down_R:  //������
		CAR_R90_CMD   =  true;
		break;
	case up_R:	//������
		CAR_L90_CMD   =  true;
		break;
	case keep_R:
		CAR_R90_CMD   =  false;
		CAR_L90_CMD   =  false;
		//���ְ�����λ������ת90�����ִ��
		car->car_r90_run_flag = CMD_RUN_OK;  
		car->car_l90_run_flag = CMD_RUN_OK;  		
	default:
		break;		
	}
}
/**
  * @Name    
  * @brief  ң�����󲦸�״̬��� 
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void RC_S1_Status_Check(car_t *car)
{

	switch(rc.base_info->s1.value)
	{
	case 0x01:  //�󲦸���
		SHOOTING_FIRE_CMD = true;
		break;  
	case 0x03:	//�󲦸���
		SHOOTING_FIRE_CMD = false;
	//����λ�ù��У������ִ��
		car->shooting_fire_run_flag = CMD_RUN_OK; 
		break;
	case 0x02:	//�󲦸���
		SHOOTING_FIRE_CMD = false;
		break;
	default:
		break;
	}

}

/*-----------------------------��������------------------------------------*/

/**
 * @brief shift����״̬���
 * 
 * @param car 
 */
void KEY_Shift_Status_Check(car_t *car)
{
	switch (rc.base_info->Shift.status)
	{
	case relax_K://����
		break;
	case down_K://����
		car->car_move_mode = gyro_CAR;
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief ctrl����״̬���
 * 
 * @param car 
 */
void KEY_Ctrl_Status_Check(car_t *car)
{
	switch (rc.base_info->Ctrl.status)
	{
	case relax_K://����
		break;
	case down_K://����
		if (car->car_move_mode == mec_CAR)//�����ǻ�еģʽ���˻�������ģʽ
		{
			car->car_move_mode = gyro_CAR;
		}
		else//���ڲ��ǻ�еģʽ�������еģʽ
		{
			car->car_move_mode = mec_CAR;
		}
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief F����״̬���
 * 
 * @param car 
 */
void KEY_F_Status_Check(car_t *car)
{
	switch (rc.base_info->F.status)
	{
	case relax_K://����
		break;
	case down_K://����
		if (car->car_move_mode == cycle_CAR)//������С����ģʽ���л���������ģʽ
		{
			car->car_move_mode = gyro_CAR;
		}
		else//���ڲ���С���ݣ�����С����
		{
			car->car_move_mode = cycle_CAR;
		}
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief E����״̬��� CAR_R90_CMD
 * 
 * @param car 
 */
void KEY_E_Status_Check(car_t *car)
{
	switch(rc.base_info->E.status)
	{
	case relax_K://����
		CAR_R90_CMD   =  false;
		car->car_r90_run_flag = CMD_RUN_OK;
		break;
	case down_K://����
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		CAR_R90_CMD   =  true;
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief Q����״̬��� CAR_L90_CMD
 * 
 * @param car 
 */
void KEY_Q_Status_Check(car_t *car)
{
	switch(rc.base_info->Q.status)
	{
	case relax_K://����
		CAR_L90_CMD   =  false;
		car->car_l90_run_flag = CMD_RUN_OK;
		break;
	case down_K://����
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		CAR_L90_CMD   =  true;
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief V����״̬��� GIM_180_CMD
 * 
 * @param car 
 * @note �ڻ�еģʽ�°���ͷ���Զ�����������ģʽ
 */
void KEY_V_Status_Check(car_t *car)
{
	switch(rc.base_info->V.status)
	{
	case relax_K://����
		GIM_180_CMD   =  false;
		car->gim_180_run_flag = CMD_RUN_OK;
		break;
	case down_K://����
		if(car->car_move_mode == mec_CAR)
		{
			car->car_move_mode = gyro_CAR;
		} 
		GIM_180_CMD   =  true;
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		break;
	case long_press_K://����
		break;
	default:
		break;
	}
}

/**
 * @brief ������״̬��� SHOOTING_FIRE_CMD SHOOTING_FIRING_CMD
 * 
 * @param car 
 */
void KEY_ML_Status_Check(car_t *car)
{
	switch(rc.base_info->mouse_btn_l.status)
	{
	case relax_K://����
		SHOOTING_FIRE_CMD = false;
		SHOOTING_FIRING_CMD = false;
		car->shooting_fire_run_flag = CMD_RUN_OK;
		break;
	case down_K://����
		break;
	case up_K://̧��
		break;
	case short_press_K://�̰�
		SHOOTING_FIRE_CMD = true;
		break;
	case long_press_K://����
		SHOOTING_FIRING_CMD = true;
		break;
	default:
		break;
	}
}