/*
 * humidity_write_stm.c
 *
 *  Created on: Nov 23, 2019
 *      Author: Student235325
 */


/* Includes ------------------------------------------------------------------*/
#include "common_blesvc.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct{
  uint16_t	service_hdle;				        /**< Service handle */
  uint16_t	write_client_to_server_char_hdle;	  /**< Characteristic handle */
//  uint16_t	TemperatureNotifyServerToClientCharHdle;	/**< Characteristic handle */
  uint16_t  reboot_req_char_Hdle;                /**< Characteristic handle */
}Humidity_Setter_Context_t;

/* Private defines -----------------------------------------------------------*/
#define UUID_128_SUPPORTED  1

#if (UUID_128_SUPPORTED == 1)
#define BM_UUID_LENGTH  UUID_TYPE_128
#else
#define BM_UUID_LENGTH  UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE    (3)

/* Private variables ---------------------------------------------------------*/
#if(BLE_CFG_OTA_REBOOT_CHAR != 0)
	#if (UUID_128_SUPPORTED == 1)
		static const uint8_t BM_REQ_CHAR_UUID[16] = {0x19, 0xed, 0x82, 0xae,
                                       0xed, 0x21, 0x4c, 0x9d,
                                       0x41, 0x45, 0x22, 0x8e,
                                       0x11, 0xFE, 0x00, 0x00};
	#else
		static const uint8_t BM_REQ_CHAR_UUID[2] = {0x11, 0xFE};
	#endif
#endif

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static Humidity_Setter_Context_t hum_set_context;

/* Private function prototypes -----------------------------------------------*/
static SVCCTL_EvtAckStatus_t HumiditySetterEventHandler(void *event_ptr);

/* Private functions ----------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)

/* Hardware Characteristics Service */
#define COPY_HUMIDITY_SET_SERVICE_UUID(uuid_struct)       COPY_UUID_128(uuid_struct,0x27,0xa5,0xc6,0xc6,0xbd,0x13,0x49,0xd5,0xb7,0x30,0x7f,0x81,0xA5,0x88,0x70,0xf9)
#define COPY_HUMIDITY_SET_CHAR_UUID(uuid_struct)    COPY_UUID_128(uuid_struct,0x00,0x00,0xAA,0xCC,0x8e,0x22,0x45,0x41,0x9d,0x4c,0x21,0xed,0xae,0x82,0xed,0x19)

static SVCCTL_EvtAckStatus_t HumiditySetterEventHandler(void *event_ptr)
{
	SVCCTL_EvtAckStatus_t return_value;
	hci_event_pckt *event_pckt_ptr;
	evt_blue_aci *blue_evt_ptr;
	aci_gatt_attribute_modified_event_rp0 *attribute_modified_ptr;
	Humidity_Setter_APP_Notification_Evt_t notification;

	return_value = SVCCTL_EvtNotAck;
	event_pckt_ptr = (hci_event_pckt *)(((hci_uart_pckt*)event_ptr)->data);

	switch (event_pckt_ptr->evt)
	{
		case EVT_VENDOR:
		{
			blue_evt_ptr = (evt_blue_aci*)event_pckt_ptr->data;

			switch (blue_evt_ptr->ecode)
			{
				case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
				{
					attribute_modified_ptr = (aci_gatt_attribute_modified_event_rp0*)blue_evt_ptr->data;

					if(attribute_modified_ptr->Attr_Handle == (hum_set_context.write_client_to_server_char_hdle + 1))
					{
						BLE_DBG_TEMPLATE_STM_MSG("-- GATT : WRITE CHAR INFO RECEIVED\n");

						notification.evt_opcode = HUMIDITY_SETTER_WRITE_EVT;
						notification.data_transferred.length = attribute_modified_ptr->Attr_Data_Length;
						notification.data_transferred.payload_ptr = attribute_modified_ptr->Attr_Data;

						HumiditySetterAppNotification(&notification);
					}

					else if(attribute_modified_ptr->Attr_Handle == (hum_set_context.reboot_req_char_Hdle + 1))
					{
						BLE_DBG_TEMPLATE_STM_MSG("-- GATT : REBOOT REQUEST RECEIVED\n");

						notification.evt_opcode = HUMIDITY_SETTER_BOOT_REQUEST_EVT;
						notification.data_transferred.length = attribute_modified_ptr->Attr_Data_Length;
						notification.data_transferred.payload_ptr = attribute_modified_ptr->Attr_Data;


						HumiditySetterAppNotification(&notification);
					}
			} // EVT_BLUE_GATT_ATTRIBUTE_MODIFIED - END
			break;

			default:
				break;

			} //switch blue_evt_ptr->ecode - END
			break;
		} // case EVT_VENDOR - END

		default:
			break;
	} // switch (event_pckt_ptr->evt) - END

	return return_value;
}	/* end SVCCTL_EvtAckStatus_t */


void SVCCTL_InitHumiditySetSvc(void)
{
	  Char_UUID_t  uuid16;

	  SVCCTL_RegisterSvcHandler(HumiditySetterEventHandler);

	  COPY_HUMIDITY_SET_SERVICE_UUID(uuid16.Char_UUID_128);
	  aci_gatt_add_service(UUID_TYPE_128,
	                       (Service_UUID_t *) &uuid16,
	                       PRIMARY_SERVICE,
	                       8, /*Max_Attribute_Records*/
	                       &(hum_set_context.service_hdle));

	    /**
	     *  Add Write Characteristic
	     */

	  COPY_HUMIDITY_SET_CHAR_UUID(uuid16.Char_UUID_128);
	  aci_gatt_add_char(hum_set_context.service_hdle,
	                    UUID_TYPE_128, &uuid16,
	                    2,
	                    CHAR_PROP_WRITE_WITHOUT_RESP|CHAR_PROP_READ,
	                    ATTR_PERMISSION_NONE,
	                    GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
	                    10, /* encryKeySize */
	                    1, /* isVariable */
	                    &(hum_set_context.write_client_to_server_char_hdle));


	  #if(OTA_REBOOT_SUPPORT != 0)
	   /**
		*  Add Boot Request Characteristic
		*/
	  aci_gatt_add_char(hum_set_context.service_hdle,
						 BM_UUID_LENGTH,
						 (Char_UUID_t *)BM_REQ_CHAR_UUID,
						 BM_REQ_CHAR_SIZE,
						 CHAR_PROP_WRITE_WITHOUT_RESP,
						 ATTR_PERMISSION_NONE,
						 GATT_NOTIFY_ATTRIBUTE_WRITE,
						 10,
						 0,
						 &(hum_set_context.reboot_req_char_Hdle));
	#endif
}

tBleStatus HumiditySetterAppUpdateChar(uint16_t uuid, uint8_t *payload_ptr)
{
	//TODO Check if this is necessary
}
