/**
 * @file control.c
 * @author Isaac (1812924685@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "control.h"

/*kp		 ki		  kd		b_err	  int_max		iout_max	out_max*/
control_t control = 
{
  /*6020 pid*/
  .GM6020_speed_pid_param     = {0,10,0,0,2500,20000,20000},
  .GM6020_posit_out_pid_param = {0.1,0,5,0,10000,6000,300},
  .GM6020_posit_in_pid_param  = {0,10,0,0,10000,20000,20000},
  /*2006 pid*/
  .RM2006_speed_pid_param     = {0,10,0,0,2500,20000,20000},
  .RM2006_posit_in_pid_param  = {0,10,0,0,10000,20000,20000},
  .RM2006_posit_out_pid_param = {0.1,0,5,0,10000,6000,300},
  /*3508 pid*/
  .RM3508_speed_pid_param     = {0,10,0,0,2500,20000,20000},
  .RM3508_posit_in_pid_param  = {0,10,0,0,10000,20000,20000},
  .RM3508_posit_out_pid_param = {0.1,0,5,0,10000,6000,300},


  /**
   * @brief pid类型,选用哪一套pid参数
   * PID_GM6020 
   * PID_RM2006 
   * PID_RM3508
   */
  .pid_type = PID_GM6020,
  
  /**
   * @brief 控制模式
   * 速度环：SPEED_MODE(0) 
   * 位置环：POSITION_MODE(1)
   */
  .control_mode = POSITION_MODE, 

  .target_speed = 100,//电机目标速度
  .target_position = 10000,//电机目标位置
  
};

/**
 * @brief 信息更新 task中调用
 * 
 * @param control 
 */
void Control_Info_Update(control_t *control)
{
  control->rx_info = &test_motor.rx_info;
  control->state = &test_motor.state;
}

