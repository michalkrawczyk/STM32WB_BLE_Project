/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : temperature_server_app.c
 * Description        : P2P Server Application
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* USER CODE BEGIN UserCode */
/* Includes ------------------------------------------------------------------*/
#include "temperature_server_app.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "stm32_seq.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
	uint16_t time_stamp;
	uint16_t value;
} TemperatureCharValue_t;

typedef struct
{
  uint8_t  NotificationStatus;
  uint16_t Parameter;
  TemperatureCharValue_t Temperature;
//  int16_t ChangeStep; 					//Additionally TODO: Find naming convention for structs
  uint8_t Update_timer_ID;
} Temperature_Server_App_Context_t;


/* Private defines -----------------------------------------------------------*/
#define TEMPERATURE_CHANGE_STEP 10
#define TEMPERATURE_CHANGE_PERIOD	(1 * 1000 * 1000 / CFG_TS_TICK_VAL) 	// 1s
//#define TEMPERATURE_VALUE_MAX_THRESHOLD 350
//#define TEMPERATURE_VALUE_MIN_THRESHOLD 100   // For Debug

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * START of Section BLE_APP_CONTEXT
 */
PLACE_IN_SECTION("BLE_APP_CONTEXT") static Temperature_Server_App_Context_t temperature_server_app_context;

/**
 * END of Section BLE_APP_CONTEXT
 */
/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void Temperature_APP_context_Init(void);
static void Temperature_Send_Notification_Task(void);

static void TemperatureChange_Timer_Callback(void)
{
	UTIL_SEQ_SetTask(1 << CFG_TASK_TEMPERATURE_NOTIFY, CFG_SCH_PRIO_0);
}



/* Public functions ----------------------------------------------------------*/
void TEMPERATURE_STM_App_Notification(Temperature_STM_App_Notification_evt_t *pNotification)
{
  switch(pNotification->Temperature_Evt_Opcode)
  {
    case TEMPERATURE_STM_NOTIFY_ENABLED_EVT:
      temperature_server_app_context.NotificationStatus = 1;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION ENABLED\n");
      APP_DBG_MSG(" \n\r");
#endif

      HW_TS_Start(temperature_server_app_context.Update_timer_ID, TEMPERATURE_CHANGE_PERIOD);

      break; /* TEMPLATE_STM_NOTIFY_ENABLED_EVT */

    case TEMPERATURE_STM_NOTIFY_DISABLED_EVT:
      temperature_server_app_context.NotificationStatus = 0;
#if(CFG_DEBUG_APP_TRACE != 0)
      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFICATION DISABLED\n");
      APP_DBG_MSG(" \n\r");
#endif

      HW_TS_Stop(temperature_server_app_context.Update_timer_ID);

      break; /* TEMPLATE_STM_NOTIFY_DISABLED_EVT */
//TODELETE
//    case TEMPLATE_STM_WRITE_EVT:
//#if(CFG_DEBUG_APP_TRACE != 0)
//      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : WRITE EVENT RECEIVED\n");
//      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : 0x%x\n",pNotification->DataTransfered.pPayload[0]);
//      APP_DBG_MSG(" \n\r");
//#endif
//      if(pNotification->DataTransfered.pPayload[0] == 0x00)
//      {
//#if(CFG_DEBUG_APP_TRACE != 0)
//      APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : START TASK 2 \n");
//#endif
//      }
//      break; /* TEMPLATE_STM_WRITE_EVT */

#if(BLE_CFG_OTA_REBOOT_CHAR != 0)       
    case TEMPERATURE_STM_BOOT_REQUEST_EVT:
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : BOOT REQUESTED\n");
    APP_DBG_MSG(" \n\r");
#endif

      *(uint32_t*)SRAM1_BASE = *(uint32_t*)pNotification->DataTransfered.pPayload;
      NVIC_SystemReset();

      break; /* TEMPLATE_STM_BOOT_REQUEST_EVT */
#endif
      
    default:
      break; /* DEFAULT */
  }

  return;
}

void Temperature_APP_Init(void)
{
	UTIL_SEQ_RegTask(1 << CFG_TASK_TEMPERATURE_NOTIFY, UTIL_SEQ_RFU, Temperature_Send_Notification_Task);

	HW_TS_Create(CFG_TIM_PROC_ID_ISR,
			&(temperature_server_app_context.Update_timer_ID),
			hw_ts_Repeated,
			TemperatureChange_Timer_Callback);
  /**
   * Initialize Template application context
   */
  temperature_server_app_context.NotificationStatus=0;
  Temperature_APP_context_Init();
  return;
}

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/
static void Temperature_APP_context_Init(void)
{
  temperature_server_app_context.Parameter = 0;

  temperature_server_app_context.Temperature.time_stamp = 0;
  temperature_server_app_context.Temperature.value = 0;

//  temperature_server_app_context.ChangeStep = TEMPERATURE_CHANGE_STEP;
}

static void Temperature_Send_Notification_Task(void)
{
  uint8_t value[4];

  value[0] = (uint8_t)(temperature_server_app_context.Temperature.time_stamp & 0x00FF);
  value[1] = (uint8_t)(temperature_server_app_context.Temperature.time_stamp >> 8);
  value[2] = (uint8_t)(temperature_server_app_context.Temperature.value & 0x00FF);
  value[3] = (uint8_t)(temperature_server_app_context.Temperature.value >> 8);

  //TODO:ADD DHT READINGS HERE
  //TEMPLATE_Server_App_Context.Temperature.value = //add getter from DHT;

  TEMPLATE_Server_App_Context.Temperature.time_stamp += TEMPERATURE_STEP;

  // DEBUG FUNCTION
//  temperature_server_app_context.Temperature.value += temperature_server_app_context.ChangeStep;
//  temperature_server_app_context.Temperature.time_stamp += TEMPERATURE_CHANGE_STEP;
//
//  if (temperature_server_app_context.Temperature.value > TEMPERATURE_VALUE_MAX_THRESHOLD)
//  {
//	  temperature_server_app_context.ChangeStep = -TEMPERATURE_CHANGE_STEP;
//  }
//  else if (temperature_server_app_context.Temperature.value < TEMPERATURE_VALUE_MIN_THRESHOLD)
//  {
//	  temperature_server_app_context.ChangeStep = +TEMPERATURE_CHANGE_STEP;
//  }


  if(temperature_server_app_context.NotificationStatus)
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : NOTIFY CLIENT WITH NEW PARAMETER VALUE \n ");
    APP_DBG_MSG(" \n\r");
#endif
    TEMPERATURE_STM_App_Update_Char(0x0000,(uint8_t *)&value);
  }
  else
  {
#if(CFG_DEBUG_APP_TRACE != 0)
    APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : CAN'T INFORM CLIENT - NOTIFICATION DISABLED\n ");
#endif
  }

  return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/* USER CODE END UserCode */
