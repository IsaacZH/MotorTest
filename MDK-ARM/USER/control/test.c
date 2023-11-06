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
void Test_Update(test_t *test);
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
	test->base_info.output = test->motor->c_speed(test->motor,test->base_info.target_speed);
}

/**
 * @brief �������ļ��и��²���
 * 
 */
void Test_Update(test_t *test)
{
  test->base_info.target_speed = config_test.target_speed;
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
    test->base_info.target_speed = config_test.target_speed;
    Test_Work_Normal(test);
  }
  else
  {
    /* code */
  }
  
}












