#include "SLS.h"
#include "hal_key.h"
#include "hal_lcd.h"
#include "hal_led.h"
#include "hal_sleep.h"
#include "hal_timer.h"
#include "hal_types.h"
#include "hal_uart.h"
#include "hal_defs.h"
#include "znp_app.h"
#include "OSAL.h"

uint8 SLS_TaskID;

void initUart(halUARTCBack_t pf);
void SLS_UartCback(uint8 port, uint8 event);

static void LedTest( int i, unsigned char val );

static void LedTest( int i, unsigned char val ){
  P0DIR |= 0x03;
  if(i==1)
    P0_1 = val;
  else
    P0_0 = val;
}

/**************************************************************************************************
* @fn      Hal_Init
*
* @brief   Hal Initialization function.
*
* @param   task_id - Hal TaskId
*
* @return  None
**************************************************************************************************/
void SLS_Init(uint8 task_id)
{
	/* Register task ID */
	SLS_TaskID = task_id;
    //initUart(SLS_UartCback);
}

void SLS_UartCback(uint8 port, uint8 event)
{
        
	switch (event) {
	case HAL_UART_RX_FULL:
	case HAL_UART_RX_ABOUT_FULL:
	case HAL_UART_RX_TIMEOUT:
		break;

	case HAL_UART_TX_EMPTY:
		break;

	default:
		break;
	}
}

void initUart(halUARTCBack_t pf)
{
  	  halUARTCfg_t uartConfig;

	  uartConfig.configured = TRUE;
	  uartConfig.baudRate = HAL_UART_BR_9600;//ZNP_UART_BAUD;
	  uartConfig.flowControl = TRUE;
	  uartConfig.flowControlThreshold = 48;// HAL_UART_FLOW_THRESHOLD;
	  uartConfig.rx.maxBufSize = 128;// HAL_UART_RX_BUF_SIZE;
	  uartConfig.tx.maxBufSize = 128;// HAL_UART_TX_BUF_SIZE;
	  uartConfig.idleTimeout = 6;// HAL_UART_IDLE_TIMEOUT;
	  uartConfig.intEnable = TRUE;
	  uartConfig.callBackFunc = pf;
	  HalUARTOpen(HAL_UART_PORT_0, &uartConfig);
}

void TestUARTFunc(void){
  
         uint8 buf[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35 };
         int i =10;
 	while (i-- >0)
 	{
 		HalUARTWrite(HAL_UART_PORT_1, buf, 6);
 	}
}

uint16 SLS_ProcessEvent(uint8 task_id, uint16 events)
{
	uint8 *msgPtr;

	(void)task_id;  // Intentionally unreferenced parameter

	if (events & SYS_EVENT_MSG)
	{
		msgPtr = osal_msg_receive(SLS_TaskID);

		while (msgPtr)
		{
			/* Do something here - for now, just deallocate the msg and move on */

			/* De-allocate */
			osal_msg_deallocate(msgPtr);
			/* Next */
			msgPtr = osal_msg_receive(SLS_TaskID);
		}
		return events ^ SYS_EVENT_MSG;
	}

	return 0;
}
