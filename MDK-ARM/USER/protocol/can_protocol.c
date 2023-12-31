/* Includes ------------------------------------------------------------------*/
#include "can_protocol.h"
#include "drv_can.h"
#include "string.h"
#include "config_can.h"
#include "remote.h"
#include "device.h"
#include "motor.h"


typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t				data[8];
} CAN_RxFrameTypeDef;

CAN_RxFrameTypeDef hcan1RxFrame;
CAN_RxFrameTypeDef hcan2RxFrame;

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/**
  * @Name    HAL_CAN_RxFifo0MsgPendingCallback
  * @brief   can接受中断，在stm32f4xx_hal_can.c内弱定义
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if(hcan == &hcan1)
  {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		CAN1_RX(hcan1RxFrame.header.StdId, hcan1RxFrame.data);
  }
  else if(hcan == &hcan2)
  {
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan2RxFrame.header, hcan2RxFrame.data);
		CAN2_RX(hcan2RxFrame.header.StdId, hcan2RxFrame.data);
  }
  else 
    return;
}




/**
  * @Name    CAN_Send
  * @brief   can发送函数，在任务中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
uint8_t can_txbuff[8];
uint8_t CAN_Send(void)
{
	memset(can_txbuff,0,sizeof(can_txbuff));
	
	/*****0x200包*****/
	HAL_CAN_TxHeadeInit(CAN_0x200_ID);
	/*CAN1*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN1)
				if(device[i].motor->id.rx_id>=0x201&&device[i].motor->id.rx_id<=0x204)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));
	/*CAN2*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN2)
				if(device[i].motor->id.rx_id>=0x201&&device[i].motor->id.rx_id<=0x204)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan2,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));

	/*****0x1ff包*****/
	HAL_CAN_TxHeadeInit(CAN_0x1ff_ID);
	/*CAN1*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN1)
				if(device[i].motor->id.rx_id>=0x205&&device[i].motor->id.rx_id<=0x208)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));
	/*CAN2*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN2)
				if(device[i].motor->id.rx_id>=0x205&&device[i].motor->id.rx_id<=0x208)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan2,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));
	
	/*****0x2ff包*****/
	HAL_CAN_TxHeadeInit(CAN_0x2ff_ID);
	/*CAN1*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN1)
				if(device[i].motor->id.rx_id>=0x209&&device[i].motor->id.rx_id<=0x20B)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));
	/*CAN2*/
	if(RC_ONLINE)
	{
		/*发送电机筛选遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->id.drive_type == M_CAN2)
				if(device[i].motor->id.rx_id>=0x209&&device[i].motor->id.rx_id<=0x20B)
					device[i].motor->tx(device[i].motor,can_txbuff,8);
	}
	if(HAL_CAN_AddTxMessage(&hcan2,&CAN_TxHeadeType,can_txbuff,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK)
		return HAL_ERROR;
	memset(can_txbuff,0,sizeof(can_txbuff));
	
	return HAL_OK;
}


