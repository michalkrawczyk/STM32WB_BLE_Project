/*
 * humidity_read_app.c
 *
 *  Created on: 21 lis 2019
 *      Author: Student235325
 */
/* Includes ------------------------------------------------------------------*/
#include "../Inc/humidity_read_app.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "stm32_seq.h"

/* Private typedef -----------------------------------------------------------*/

typedef struct
{
	uint16_t time_stamp;
	uint16_t value;
} HumidityReadCharValue_t;

typedef struct
{
	uint8_t notification_status;
	uint16_t parameter;
	HumidityReadCharValue_t humidity;
	uint8_t update_timer_id;
} HumidityReaderAppContext_t;

/* Private defines -----------------------------------------------------------*/
#define HUMIDITY_READER_CHANGE_STEP 1
#define HUMIDITY_READER_CHANGE_PERIOD	(1 * 1000 * 1000 / CFG_TS_TICK_VAL) 	// 1s


PLACE_IN_SECTION("BLE_APP_CONTEXT") static HumidityReaderAppContext_t humidity_reader_context;

/* Private functions ---------------------------------------------------------*/

static void HumidityReaderAPPcontextInit(void);
static void HumidityReaderSendNotificationTask(void);
static void HumidityChangeTimerCallback(void);

/* Public functions ----------------------------------------------------------*/
void HumidityReaderAPPInit(void)
{
	UTIL_SEQ_RegTask(1 << CFG_TASK_HUMIDITY_READINGS_NOTIFY,
					 UTIL_SEQ_RFU,
					 HumidityReaderSendNotificationTask);

	HW_TS_Create(CFG_TIM_PROC_ID_ISR,
				 &humidity_reader_context.update_timer_id,
				 hw_ts_Repeated,
				 HumidityChangeTimerCallback);

	humidity_reader_context.notification_status = 0;
	HumidityReaderAPPcontextInit();
}


void HumidityReaderAppNotification(Humidity_Reader_App_Notification_Evt_t *notification_ptr)
{
	switch(notification_ptr->Evt_Opcode)
	{
		case HUMIDITY_READER_NOTIFY_ENABLED_EVT:
		{
			humidity_reader_context.notification_status = 1;

			HW_TS_Start(humidity_reader_context.update_timer_id,
						HUMIDITY_READER_CHANGE_PERIOD);

			break;
		}

		case HUMIDITY_READER_NOTIFY_DISABLED_EVT:
		{
			humidity_reader_context.notification_status = 0;

			HW_TS_Stop(humidity_reader_context.update_timer_id);

			break;
		}

	#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
		case TEMPERATURE_STM_BOOT_REQUEST_EVT:

			#if(CFG_DEBUG_APP_TRACE != 0)
				APP_DBG_MSG("-- TEMPLATE APPLICATION SERVER : BOOT REQUESTED\n");
				APP_DBG_MSG(" \n\r");
			#endif

			*(uint32_t*)SRAM1_BASE = *(uint32_t*);
			notification_ptr.
			NVIC_SystemReset();

			break;
	#endif

    	default:
    		break;
	}

}

/* Local functions ----------------------------------------------------------*/

static void HumidityReaderAPPcontextInit(void)
{
	humidity_reader_context.parameter = 0;

	humidity_reader_context.humidity.time_stamp = 0;
	humidity_reader_context.humidity.value = 0;

}

static void HumidityReaderSendNotificationTask(void)
{
	  uint8_t value[4];

	  value[0] = (uint8_t)(humidity_reader_context.humidity.time_stamp 	& 0x00FF);
	  value[1] = (uint8_t)(humidity_reader_context.humidity.time_stamp 	>> 8);
	  value[2] = (uint8_t)(humidity_reader_context.humidity.value 		& 0x00FF);
	  value[3] = (uint8_t)(humidity_reader_context.humidity.value 		>> 8);

	  //TODO: ADD ADC Readings from Class Here
	  //humidity_reader_context.humidity = readings
	  humidity_reader_context.humidity.time_stamp += HUMIDITY_READER_CHANGE_STEP;

	  if(humidity_reader_context.notification_status)
	  {
		#if(CFG_DEBUG_APP_TRACE != 0)
		  APP_DBG_MSG("-- HUMIDITY READ APPLICATION SERVER : NOTIFY CLIENT WITH NEW PARAMETER VALUE \n ");
		  APP_DBG_MSG(" \n\r");
		#endif

		  HumidityReader_App_Update_Char(0x0000, (uint8_t *)&value);
	  }
	  else
	  {
		#if(CFG_DEBUG_APP_TRACE != 0)
		  APP_DBG_MSG("-- HUMIDITY READ APPLICATION SERVER : CAN'T INFORM CLIENT - NOTIFICATION DISABLED\n ");
		#endif
	  }

}

static void HumidityChangeTimerCallback(void)
{
	humidity_reader_context.humidity.value +=1;
	UTIL_SEQ_SetTask(1 << CFG_TASK_HUMIDITY_READINGS_NOTIFY,
					 1);
}




