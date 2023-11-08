/*
* test
*	
*	2023.3.4
* ��̨
*/


/* Includes ------------------------------------------------------------------*/
#include "test.h"
#include "device.h"

/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

test_t test = 
{
	.motor = &test_motor,
	.work = Test_Work,
};

/* Function  body --------------------------------------------------------*/

/**
  * @Name    Test_Pid_Calculating
  * @brief   PID����
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Test_Pid_Calculating(test_t *test)
{
	if (control.control_mode == SPEED_MODE)//�ٶ�ģʽ
  {
    /* code */  
		test->base_info.output = test->motor->c_speed(test->motor,test->base_info.target_speed);

  }
  else if (control.control_mode == POSITION_MODE)//λ��ģʽ
  {
    /* code */
    test->base_info.output = test->motor->c_posit(test->motor,test->base_info.target_position);
  }
}

/**
  * @Name    Test_Work_Normal
  * @brief   �������
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Test_Work_Normal(test_t *test)
{
	Test_Pid_Calculating(test);
	
	test->motor->base_info.motor_out 	= test->base_info.output;
}


/**
  * @Name    Test_Work
  * @brief   �ܿ�
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Test_Work(test_t *test)
{
  if (RC_ONLINE)
  {
    test->base_info.target_speed = control.target_speed;
    test->base_info.target_position = control.target_position;
    Test_Work_Normal(test);
  }
  else
  {
    /* code */
  }
  
}












