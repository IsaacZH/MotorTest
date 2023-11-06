/**
  ******************************************************************************
  * @file           : vision_potocol.c
  * @brief          : 
  * @update         : 
  ******************************************************************************
  */

#include "vision_protocol.h"
#include "stdbool.h"
#include "string.h"
#include "usart.h"
#include "crc.h"

vision_tx_info_t vision_tx_info = 
{
	.SOF = 0xA5,

};

vision_rx_info_t vision_rx_info;

uint8_t vision_txBuf[30];


/**
 * @brief �Ӿ�ͨ����
 * 
 * @param rxBuf 
 * @return �յ����ݣ�DEV_OK û�е����ݣ�DEV_NO
 * @note 
 */
Dev_Work_State_e Vision_DataRx(uint8_t *rxBuf)
{
	/* ֡���ֽ��Ƿ�Ϊ0xA5 */
	if(rxBuf[0] == 0xA5)
	{
		/* ֡ͷCRC8У��*/
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			/* ֡βCRC16У�� */
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(vision_rx_info_t)) == true)
			{
				memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
				return DEV_OK;
			}
		}
	}
	return DEV_NO;
}


/**
  * @Name    Vision_DataTx
  * @brief   �Ӿ�ͨ�ŷ�(����3)
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
uint32_t vision_tx_cnt = 0;
Dev_Work_State_e Vision_DataTx(UART_HandleTypeDef *huart)
{	
//	vision_tx_cnt ++;
	memcpy(vision_txBuf, &vision_tx_info, sizeof(vision_tx_info_t));//���÷�����Ϣ
	Append_CRC8_Check_Sum(vision_txBuf, 3);//����CRC8У����
	Append_CRC16_Check_Sum(vision_txBuf, sizeof(vision_tx_info_t));//����CRC16У����
	
	if(HAL_UART_Transmit_DMA(huart,vision_txBuf,sizeof(vision_tx_info_t)) == HAL_OK)//���ڷ���
		return DEV_OK;
	
	return DEV_NO;
}   
