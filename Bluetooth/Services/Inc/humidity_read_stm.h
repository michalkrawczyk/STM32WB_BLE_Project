/*
 * humidity_read_stm.h
 *
 *  Created on: 21 lis 2019
 *      Author: Student235325
 */

#ifndef SERVICES_INC_HUMIDITY_READ_STM_H_
#define SERVICES_INC_HUMIDITY_READ_STM_H_
#ifndef tBleStatus
	#include <ble_types.h>
#endif

#ifndef uint8_t
	#include <sys/_stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

typedef enum
{
	HUMIDITY_READER_NOTIFY_ENABLED_EVT,
	HUMIDITY_READER_NOTIFY_DISABLED_EVT,
	HUMIDITY_READER_BOOT_REQUEST_EVT
} Humidity_Reader_Opcode_Evt_t;

typedef struct
{
	uint8_t *Payload_ptr;
	uint8_t Length;
} Humidity_Reader_Data_t;

typedef struct
{
	Humidity_Reader_Opcode_Evt_t 	Evt_Opcode;
	Humidity_Reader_Data_t 			Data_Transferred;
	uint16_t 						Connection_Hdle;
	uint8_t							Service_Instance;
}Humidity_Reader_App_Notification_Evt_t;

/* Exported functions ------------------------------------------------------- */

void HumidityReaderInit(void);
void HumidityReaderAppNotification(Humidity_Reader_App_Notification_Evt_t *notification_ptr); //consider moving to Server_app
tBleStatus HumidityReader_App_Update_Char(uint16_t uuid, uint8_t *payload_ptr); 						//consider moving to Server_app


#ifdef __cplusplus
}
#endif

#endif /* SERVICES_INC_HUMIDITY_READ_STM_H_ */
