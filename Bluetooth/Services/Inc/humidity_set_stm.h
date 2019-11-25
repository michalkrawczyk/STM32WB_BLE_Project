/*
 * humidity_write_stm.h
 *
 *  Created on: Nov 23, 2019
 *      Author: Student235325
 */

#ifndef SERVICES_INC_HUMIDITY_SET_STM_H_
#define SERVICES_INC_HUMIDITY_SET_STM_H_
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#ifndef tBleStatus
	#include <ble_types.h>
#endif

#ifndef uint8_t
	#include <sys/_stdint.h>
#endif
/* Typedef Definitions ------------------------------------------------------------------*/

typedef enum
{
	HUMIDITY_SETTER_READ_EVT,
	HUMIDITY_SETTER_WRITE_EVT,
	HUMIDITY_SETTER_BOOT_REQUEST_EVT,
} Humidity_Setter_Opcode_Evt_t;

typedef struct
{
	uint8_t *payload_ptr;
	uint8_t length;
} Humidity_Setter_Data_t;

typedef struct
{
	Humidity_Setter_Opcode_Evt_t 	evt_opcode;
	Humidity_Setter_Data_t 			data_transferred;
	uint16_t						connection_hdle;
	uint8_t							service_instance;
}Humidity_Setter_APP_Notification_Evt_t;


/* Exported functions ------------------------------------------------------- */

//void HumiditySetterInit(void);
void HumiditySetterAppNotification(Humidity_Setter_APP_Notification_Evt_t *notification_ptr);
tBleStatus HumiditySetterAppUpdateChar(uint16_t uuid, uint8_t *payload_ptr);

#ifdef __cplusplus
}
#endif

#endif /* SERVICES_INC_HUMIDITY_SET_STM_H_ */
