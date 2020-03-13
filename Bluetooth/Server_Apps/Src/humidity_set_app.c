/*
 * humidity_write_app.c
 *
 *  Created on: Nov 23, 2019
 *      Author: Student235325
 */

#include "../Bluetooth/Server_Apps/Inc/humidity_set_app.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "stm32_seq.h"
#include "../WateringSystem/Inc/Valve.h"

extern Watering_Set_t watering_set;

#include "main.h"

typedef struct
{
  uint16_t parameter;
} Humidity_Set_Server_App_Context_t;

/**
 * START of Section BLE_APP_CONTEXT
 */
PLACE_IN_SECTION("BLE_APP_CONTEXT") static Humidity_Set_Server_App_Context_t hum_set_app_context;

/**
 * END of Section BLE_APP_CONTEXT
 *  */

/* Private functions ---------------------------------------------------------*/

static void HumiditySetAPPcontextInit(void);
static void HumiditySetExecuteFunc(void);

/* Public functions ----------------------------------------------------------*/
void HumiditySetterAppNotification(Humidity_Setter_APP_Notification_Evt_t *notification_ptr)
{
	switch(notification_ptr->evt_opcode)
		  {
			case HUMIDITY_SETTER_WRITE_EVT:
				hum_set_app_context.parameter = notification_ptr->data_transferred.payload_ptr[0] << 8;
				hum_set_app_context.parameter += notification_ptr->data_transferred.payload_ptr[1];

				HumiditySetExecuteFunc();

				break; /* TEMPLATE_STM_WRITE_EVT */

		#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
			case HUMIDITY_SETTER_BOOT_REQUEST_EVT:
				#if(CFG_DEBUG_APP_TRACE != 0)
					APP_DBG_MSG("-- APPLICATION STOP SERVER : BOOT REQUESTED\n");
					APP_DBG_MSG(" \n\r");
				#endif

					*(uint32_t*)SRAM1_BASE = *(uint32_t*)notification_ptr->data_transferred.payload_ptr;
					NVIC_SystemReset();

					break; /* SVC_STOP_BOOT_REQUEST_EVT */
		#endif

	    default:
	      break; /* DEFAULT */
	  }
}

void HumiditySetterAPPInit(void)
{
	HumiditySetAPPcontextInit();
}

/* Local functions ----------------------------------------------------------*/
static void HumiditySetAPPcontextInit(void)
{
	hum_set_app_context.parameter = 0u;
}

static void HumiditySetExecuteFunc(void)
{
	if (hum_set_app_context.parameter > 100)
	{
		hum_set_app_context.parameter = 100;
	}

	watering_set.valves[0].humidity_limit = hum_set_app_context.parameter;

	ManageSystem(0, hum_set_app_context.parameter);
}

