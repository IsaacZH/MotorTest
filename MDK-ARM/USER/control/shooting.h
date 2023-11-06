#ifndef _SHOOTING_H
#define _SHOOTING_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "shooting_motor.h"
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//��λ����
#define LIMIT_POSIT_VALUE -160000 		//��λ����λ������������
#define LIMIT_SPEED       -8000       //��λת�٣�����
//���̲���
#define DAIL_REVERT_PISITION -5000  //���̷�תҪ�ߵĽǶȣ���תΪ����
#define DAIL_FEED_SPEED 3000  				//���̹����ٶ�
#define DAIL_STUCK_SPEED -3000    	//���̷�ת���������ٶ�(��תΪ��)
#define DAIL_MAX_WORK_TIMES 1500   //���������ʱ�䣨�жϵ����Ƿ�Ϊ�գ�
#define DAIL_MAX_F_STUCK_CNT 2    	//���������ת���������ж��Ƿ񹩵���ɣ�
/* Exported variables ---------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/


/**
 * @brief װ��״̬ö��
 * 
 */
typedef enum 
{
	load_OK = 0,		//װ�����	 
	load_NO,     		//��Ҫװ��

}shooting_load_state_e;

/**
 * @brief ����״̬ö��
 * 
 */
typedef enum
{
	DAIL_SLEEP = 0, //�������ߵȴ�ָ��
	DAIL_RELOAD,    //������ת������
	DAIL_REVERT,    //���̷�ת
}dail_state_e;

/**
 * @brief ��������Ϣ
 * 
 */
typedef struct dail
{
	float    target_speed;     //����Ŀ���ٶ�
	int32_t  target_position;  //���̷�תĿ��λ��
	uint8_t  f_stucking_cnt;   //��ǰ��ת����
	uint8_t  b_stucking_cnt;   //����ת����
	uint16_t work_times;			//���̹���ʱ��
	dail_state_e work_state;  //���̹���״̬
}dail_info_t;


/**
 * @brief ��λ״̬ö��
 * 
 */
typedef enum
{
	LIMIT_SLEEP = 0, //��λ���ߵȴ�ָ��
	LIMIT_RUNING,    //��λ����ת
}limit_state_e;

/**
 * @brief ��λ����Ϣ
 * 
 */
typedef struct limit
{
	float         target_speed;     //��λĿ���ٶ�
	int32_t       target_position;  //��λĿ��λ��
	limit_state_e work_state;  //��λ����״̬
}limit_info_t;

/** 
  * @brief  ���������Ϣ����
  */ 
typedef struct __packed 
{
	
	int16_t    	 output_dail;    //�������
	int16_t			 output_limit;   //��λ���

	dail_info_t  dail_info;//������Ϣ
	limit_info_t limit_info;//��λ��Ϣ

}shooting_base_info_t;


/**
 * @brief ��������Ϣ
 * 
 */
typedef struct shooting_class_t
{	
	/*ע����*/
	motor_t        				 *frictionLF;
	motor_t        				 *frictionRF;
	motor_t        				 *dail;
	motor_t        				 *limit;
	
	shooting_base_info_t   base_info;	

	void               	   (*work)(struct shooting_class_t *shooting);
	
	Dev_Reset_State_e			 shooting_reset_state; //�����ʼ��״̬

  shooting_load_state_e  load_state;  //����װ��״̬  װ��=load_OK
}shooting_t;


extern shooting_t shooting;



/* Exported functions --------------------------------------------------------*/

/*work*/
void Shooting_Work(shooting_t *shooting);

/*pid_cal*/
void Shooting_Pid_Calculating(shooting_t *shooting);

/*�����ʼ��ģʽ*/
void Shooting_Work_Init(shooting_t *shooting);

/*�����ģʽ*/
uint8_t Shooting_Work_Fire(shooting_t *shooting);

/*����*/
uint8_t Shooting_Work_Reloading(shooting_t *shooting);
#endif
