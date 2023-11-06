/*
* shooting
*	
*	2023.9.11
*/

/* Includes ------------------------------*/
#include "shooting.h"
#include "device.h"
#include "car.h"
#include "math_support.h"
/* Exported variables --------------------*/



shooting_t shooting = 
{
	.frictionLF = &shooting_motor[FRICTION_LF],
	.frictionRF = &shooting_motor[FRICTION_RF],
	.dail       = &shooting_motor[DAIL],
	.limit      = &shooting_motor[LIMIT],
	
	
	.work = Shooting_Work,
	
	.shooting_reset_state = DEV_RESET_NO,
	.load_state = load_NO,
};

/* Function  body --------------------------------------------------------*/

/**
 * @brief ����PID����
 * 
 * @param shooting 
 */
void Shooting_Pid_Calculating(shooting_t *shooting)
{
	//����PID����
	shooting->base_info.output_dail = shooting->dail->c_speed(shooting->dail,     \
																					shooting->base_info.dail_info.target_speed);
	//����PID����
	shooting->base_info.output_limit = shooting->limit->c_speed(shooting->limit,  \
																					shooting->base_info.limit_info.target_speed);

}

/**
 * @brief ���������ʼ�� 
 * 				
 * @param shooting 
 * @retval װ�����-> ��ʼ���ɹ� װ�����
 */
void Shooting_Work_Init(shooting_t *shooting)
{
	while (Shooting_Work_Reloading(shooting) == 1)//�������
	{
		shooting->shooting_reset_state = DEV_RESET_OK;//��ʼ���ɹ�
		shooting->load_state = load_OK;								//װ�����
	}
	
}

/**
 * @brief ����
 * 
 * @param shooting 
 * @return uint8_t ������ɣ�1 δ��ɣ�0
 */
uint8_t Shooting_Work_Reloading(shooting_t *shooting)
{
	int32_t position = shooting->dail->rx_info.angle_sum;//��������λ��
	int16_t speed    = shooting->dail->rx_info.speed;    //���������ٶ� 
	dail_info_t *info = &shooting->base_info.dail_info;
	uint8_t res = 0;
switch (info->work_state)
{
case DAIL_SLEEP://���Ѳ��̣���ʼ����
	info->work_state = DAIL_RELOAD;
	break;
case DAIL_RELOAD://��ת����
	if (motor_class_stucking_flag(shooting->dail) != 1)//��תû�ж�ת 
	{
		if (info->work_times >= DAIL_MAX_WORK_TIMES)//������ʱ,û�е�->������ɣ��˳�
		{
			/* break */
			info->work_times = 0;
			info->target_speed = 0;
			info->f_stucking_cnt = 0;
			info->work_state = DAIL_SLEEP;
			res = 1;
		}
		else//û�ж�ת�ҹ���û�г�ʱ->�е�
		{
			info->target_speed = DAIL_FEED_SPEED ;
			info->work_times++;
		}
	}
	else//��ת��ת��
	{
		info->f_stucking_cnt++;
		if (info->f_stucking_cnt < DAIL_MAX_F_STUCK_CNT)//��ת����δ����->��ת
		{
			/*��ת�µĶ�ת�Ż�ı䷴תĿ��λ��*/
			info->target_position = position + DAIL_REVERT_PISITION;//���㷴תĿ��λ��
			info->work_state = DAIL_REVERT;//���뷴תģʽ
		}
		else//�����ת�������->�����ɹ����˳�
		{
			/* break */
			info->work_times = 0;
			info->target_speed = 0;
			info->f_stucking_cnt = 0;
			info->work_state = DAIL_SLEEP;
			res = 1;
		}
	}

	break;
case DAIL_REVERT:
	if (motor_class_stucking_flag(shooting->dail) == 1)//��ת��ת��->��ת����
	{
		info->work_state = DAIL_RELOAD;
	}
	else if(position <= info->target_position)//(��תΪ��)û��ת����λ��->��ת����
	{
		info->work_state = DAIL_RELOAD;
	}
	else//û��תҲû��λ
	{
		info->target_speed = DAIL_STUCK_SPEED;//���ַ�ת�ٶ�
	}
	
	break;
default:
	break;
}

	return res;

}

/**
 * @brief �����ģʽ
 * 
 * @param shooting 
 * @return uint8_t δ��λ��0 ��λ��1
 */
uint8_t Shooting_Work_Fire(shooting_t *shooting)
{ 
	uint8_t res = 0;
	int32_t position = shooting->limit->rx_info.angle_sum;//��λ����λ��
	limit_info_t *info = &shooting->base_info.limit_info;

	switch (info->work_state)
	{
	case LIMIT_SLEEP:
		info->target_position = position + LIMIT_POSIT_VALUE;//����Ŀ��λ��
		info->work_state = LIMIT_RUNING;//��ʼ����
		break;
	case LIMIT_RUNING:
		if(position <= info->target_position)//������Ϊ������λ
		{
			/*break*/
			info->target_speed = 0;//Ŀ���ٶ�Ϊ0
			info->work_state = LIMIT_SLEEP;//��������
			res = 1;//����1
		}
		else//δ��λ
		{
			info->target_speed = LIMIT_SPEED;//�����ٶ�
		}
		break;
	default:
		break;
	}
	return res;
}

/**
 * @brief �ܿ�
 * 
 * @param shooting 
 */
void Shooting_Work(shooting_t *shooting)
{
	/*������Ӧ*/
	if(SHOOTING_FIRE_CMD == true &&						//������
		shooting->load_state == load_OK && 		//װ�����
		car.shooting_fire_run_flag == CMD_RUN_OK ||//��������
		car.shooting_fire_run_state == CMD_RUNING)//��������ִ��
	{
		if (car.shooting_fire_run_state != CMD_RUNING)//�״ν������л�����״̬
		{
			car.shooting_fire_run_state = CMD_RUNING;
		}
		
		while (Shooting_Work_Fire(shooting) == 1)//����ִ�����
		{
			shooting->load_state = load_NO;//��Ҫװ��
			car.shooting_fire_run_state = CMD_FINISH;//����ִ�����
			car.shooting_fire_run_flag = CMD_RUN_NO;//�ȴ�������λ
		}	
	}
	
//��������
	if(SHOOTING_FIRING_CMD == true)
	{
		shooting->base_info.limit_info.target_speed = LIMIT_SPEED;
		Shooting_Work_Reloading(shooting);
		car.shooting_firing_run_state = CMD_RUNING;
	}
	else if(car.shooting_firing_run_state == CMD_RUNING)
	{
		shooting->base_info.limit_info.target_speed = 0;
		car.shooting_firing_run_state = CMD_FINISH;
		shooting->load_state = load_NO;
	}

//������Ʋ���
	if(!RC_ONLINE)//�ؿ�
	{
		shooting->base_info.output_dail = 0 ;
		shooting->base_info.output_limit = 0 ;
		shooting->shooting_reset_state = DEV_RESET_NO;//��Ҫ��ʼ��
	}
	else if(shooting->shooting_reset_state != DEV_RESET_OK)//�����ʼ��
	{
		Shooting_Work_Init(shooting);
	}
	else if(shooting->load_state == load_NO)//����Ƿ���Ҫװ��
	{
		while (Shooting_Work_Reloading(shooting) == 1)
		{
			shooting->load_state = load_OK;
		}
	}

	if(RC_ONLINE)//����
	{
		Shooting_Pid_Calculating(shooting); 
		shooting->dail->base_info.motor_out = shooting->base_info.output_dail;
		shooting->limit->base_info.motor_out = shooting->base_info.output_limit;

	}
}

