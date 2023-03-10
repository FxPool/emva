/**
  ******************************************************************************
  * @file    adp_mc_usb_hid.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   header file for the usbd_hid_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
#ifndef __USB_HID_USER__
#define __USB_HID_USER__


/*回调函数结构体*/
typedef struct
{
	void (*DataCb)(uint8* Buf, uint32 Len);
}ADP_USB_HID_t;

/*USB数据发送*/
void ADP_USB_HID_Transmit_FS(uint8* Buf, uint16 Len);
void ADP_USB_HID_Transmit_String(const char* format,...);
/*USB 数据回调设置*/
void ADP_USB_HID_SetDataCb(void (*arg)(uint8* Buf, uint32 Len));
/*USB VCP初始化*/
void ADP_USB_HID_Initialize(void);

#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_HID
  * @brief This file is the Header file for USBD_msc.c
  * @{
  */ 


/** @defgroup USBD_HID_Exported_Defines
  * @{
  */ 
#define USB_HID_CONFIG_DESC_SIZ       41
#define USB_HID_DESC_SIZ              9
#define HID_MOUSE_REPORT_DESC_SIZE    74

#define HID_DESCRIPTOR_TYPE           0x21
#define HID_REPORT_DESC               0x22

#define HID_HS_BINTERVAL              0x07
#define HID_FS_BINTERVAL              0x0A

#define HID_REQ_SET_PROTOCOL          0x0B
#define HID_REQ_GET_PROTOCOL          0x03

#define HID_REQ_SET_IDLE              0x0A
#define HID_REQ_GET_IDLE              0x02

#define HID_REQ_SET_REPORT            0x09
#define HID_REQ_GET_REPORT            0x01
/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */


/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  USBD_HID_cb;
/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
uint8 USBD_HID_SendReport (USB_OTG_CORE_HANDLE  *pdev, 
                                 uint8 *report,
                                 uint16 len);

uint32 USBD_HID_GetPollingInterval (USB_OTG_CORE_HANDLE *pdev);

/**
  * @}
  */ 

#endif

