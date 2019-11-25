/*
 * humidity_read_stm.c
 *
 *  Created on: 21 lis 2019
 *      Author: Student235325
 */


/* Includes ------------------------------------------------------------------*/

#include "common_blesvc.h"


typedef struct
{
	uint16_t Service_Hdle;
	uint16_t Notify_Server_to_Client_Char_Hdle;
	uint16_t Reboot_Req_Char_Hdle;

}Humidity_Reader_Context_t;				//consider move to .h

/* Private defines -----------------------------------------------------------*/

#define UUID_128_SUPPORTED 1

#if (UUID_128_SUPPORTED == 1)
	#define BM_UUID_LENGTH 		UUID_TYPE_128
#else
	#define BM_UUID_LENGTH 		UUID_TYPE_16
#endif

#define BM_REQ_CHAR_SIZE		(3)

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

PLACE_IN_SECTION("BLE_DRIVER_CONTEXT") static Humidity_Reader_Context_t humidity_reader_context;

/* Private function prototypes -----------------------------------------------*/

static SVCCTL_EvtAckStatus_t Humidity_Reader_Evt_Handler(void *event_ptr);

/* Private functions ----------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
	do {\
		uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
			uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
				uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
					uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
	}while(0)


#define COPY_HUMIDITY_READER_SERVICE_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x00,0x00,0x00,0x01,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB)

#define COPY_HUMIDITY_READER_NOTIFY_UUID(uuid_struct) COPY_UUID_128(uuid_struct,0x00,0x06,0x00,0x01,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5F,0x9B,0x34,0xFB)


static SVCCTL_EvtAckStatus_t Humidity_Reader_Evt_Handler(void *event_ptr)
{
	SVCCTL_EvtAckStatus_t return_value;
	hci_event_pckt *event_pckt_ptr;
	evt_blue_aci *blue_evt_ptr;
	aci_gatt_attribute_modified_event_rp0 *attribute_modified_ptr;
	Humidity_Reader_App_Notification_Evt_t notification;

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

					if(attribute_modified_ptr->Attr_Handle == (humidity_reader_context.Notify_Server_to_Client_Char_Hdle + 2))
					{
						return_value = SVCCTL_EvtAckFlowEnable;

						if(attribute_modified_ptr->Attr_Data[0] & COMSVC_Notification)
						{
							notification.Evt_Opcode = HUMIDITY_READER_NOTIFY_ENABLED_EVT;

							HumidityReaderAppNotification(&notification);
						}
						else
						{
							notification.Evt_Opcode = HUMIDITY_READER_NOTIFY_ENABLED_EVT;

							HumidityReaderAppNotification(&notification);
						}
					}
					else if(attribute_modified_ptr->Attr_Handle == (humidity_reader_context.Reboot_Req_Char_Hdle + 1))
					{
						BLE_DBG_TEMPLATE_STM_MSG("-- GATT : REBOOT REQUEST RECEIVED\n");

						notification.Evt_Opcode = HUMIDITY_READER_BOOT_REQUEST_EVT;
						notification.Data_Transferred.Length = attribute_modified_ptr->Attr_Data_Length;
						notification.Data_Transferred.Payload_ptr = attribute_modified_ptr->Attr_Data;

						HumidityReaderAppNotification(&notification);
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


void SVCCTL_InitHumidityReadSvc(void)
{

  Char_UUID_t  uuid16;

  /**
   *	Register the event handler to the BLE controller
   */
  SVCCTL_RegisterSvcHandler(Humidity_Reader_Evt_Handler);

    /**
     * Max_Attribute_Records = 2*no_of_char + 1
     * service_max_attribute_record = 1 for Template service +
     *                                2 for Template Write characteristic +
     *                                2 for Template Notify characteristic +
     *                                1 for client char configuration descriptor +
     *
     */

    COPY_HUMIDITY_READER_SERVICE_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_service(UUID_TYPE_128,
                      (Service_UUID_t *) &uuid16,
                      PRIMARY_SERVICE,
                      8, /*Max_Attribute_Records*/
                      &(humidity_reader_context.Service_Hdle));

    /**
     *   Add Notify Characteristic
     */
    COPY_HUMIDITY_READER_NOTIFY_UUID(uuid16.Char_UUID_128);
    aci_gatt_add_char(humidity_reader_context.Service_Hdle,
                      UUID_TYPE_128, &uuid16,
                      4,
                      CHAR_PROP_NOTIFY,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable: 1 */
                      &(humidity_reader_context.Notify_Server_to_Client_Char_Hdle));


 #if(OTA_REBOOT_SUPPORT != 0)
    /**
     *  Add Boot Request Characteristic
     */
   aci_gatt_add_char(humidity_reader_context.Service_Hdle,
                      BM_UUID_LENGTH,
                      (Char_UUID_t *)BM_REQ_CHAR_UUID,
                      BM_REQ_CHAR_SIZE,
                      CHAR_PROP_WRITE_WITHOUT_RESP,
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE,
                      10,
                      0,
                      &(humidity_reader_context.Reboot_Req_Char_Hdle));
#endif
  return;
}

tBleStatus HumidityReader_App_Update_Char(uint16_t uuid, uint8_t *payload_ptr)
{
	/**
	 * @brief  Characteristic update
	 * @param  UUID: UUID of the characteristic
	 * @param  Service_Instance: Instance of the service to which the characteristic belongs
	 */
  tBleStatus result = BLE_STATUS_INVALID_PARAMS;
  switch(uuid)
  {
    case 0x0000:

     result = aci_gatt_update_char_value(humidity_reader_context.Service_Hdle,
                             humidity_reader_context.Notify_Server_to_Client_Char_Hdle,
                              0, /* charValOffset */
                             4, /* charValueLen */
                             (uint8_t *)  payload_ptr);

      break;

    default:
      break;
  }

  return result;
}
