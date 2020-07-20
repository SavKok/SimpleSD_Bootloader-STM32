/**
  ******************************************************************************
  * @file    SimpleSD_bootloader.h
  * @author  Savvas Kokkinidis - sabbaskok@hotmail.com
  * @version V1.0
  * @date    14-July-2020
  * @brief   This file contains the common defines and functions prototypes related
  *          to SimpleSD_bootloader
  ******************************************************************************
  * @Copyright 2020 Savvas Kokkinidis
  ******************************************************************************
  */

#ifndef __SIMPLESD_BOOTLOADER_H
#define __SIMPLESD_BOOTLOADER_H


#ifdef __cplusplus
extern "C" {
#endif

/* Define the target MCU */
#define STM32F429

/* Define the value for CRC calculation using peripheral */
#define CRC_USING_PERIPHERAL 1

/* Define the value for CRC calculation using function */
#define CRC_USING_FUNCTION   0

/* Define the method of CRC calculation */
#define CRC_CALCULATION_METHOD CRC_USING_PERIPHERAL

/* Enable or disable the internal WDT */
#define SD_WATCHDOG_RUNNING	 1

/* Define the default polynomial used on STM32 devices */
#define STM32_POLYNOMIAL ((uint32_t)0x04C11DB7)

/* Set the polynomial that will be used on CRC calculation using software function */
#define CRC_POLYNOMIAL STM32_POLYNOMIAL

/* Set the time that code will wait for trigger start firmware upgrade */
#define BOOTLOADER_TRIGGER_TIME 100

#ifdef STM32F429
	/* Base address of the Flash sectors Bank 1 */
	#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
	#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

	/* Base address of the Flash sectors Bank 2 */
	#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 12, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 13, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 14, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 15, 16 Kbytes */
	#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 16, 64 Kbytes */
	#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 17, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 18, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 19, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 20, 128 Kbytes  */
	#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 21, 128 Kbytes  */
	#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 22, 128 Kbytes */
	#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 23, 128 Kbytes */
#else
	#error "Define the target MCU..."
#endif

/* Main firmware base address */
#define APPLICATION_START_ADDRESS ((uint32_t)0x08020000)

/* Main firmware end address [including the CRC] */
#define APPLICATION_END_ADDRESS ((uint32_t)0x081FFFFF)

/* Main firmware CRC size */
#define APPLICATION_CRC_SIZE 4

/* Main firmware size without CRC*/
#define APPLICATION_CRC_CALCULATION_SIZE ((uint32_t)(APPLICATION_END_ADDRESS - APPLICATION_START_ADDRESS - APPLICATION_CRC_SIZE + 1)/4)

/* Main firmware CRC address */
#define APPLICATION_CRC_ADDRESS (APPLICATION_END_ADDRESS-APPLICATION_CRC_SIZE+1)

#define APPLICATION_FS_DIR "/"

#define APPLICATION_BIN_FILENAME "Firmware.bin"

#define SDSimple_CD_Detect_Samples 10

#define SDSimple_CD_Detect_Level 0

#define SDSimple_LED_Pin  GPIO_PIN_13
#define SDSimple_LED_Port GPIOG

#define SDSimple_CD_Pin  GPIO_PIN_8
#define SDSimple_CD_Port GPIOC



enum SimpleSD_ErrorCodes
{
	SIMPLESD_OK = 0,      			 	/* Success */
	SIMPLESD_NO_SD,		 				/* No SD detected */
	SIMPLESD_FS_MOUNT_ERROR,		 	/* FS Mount error */
	SIMPLESD_FS_OPEN_ERROR,			 	/* FS Open error */
	SIMPLESD_FS_READ_ERROR,			 	/* FS Read error */
	SIMPLESD_FLASH_ERASE_ERROR,		 	/* Flash Erase error */
	SIMPLESD_FLASH_WRITE_ERROR,		 	/* Flash Write error */
	SIMPLESD_FLASH_WRITE_COMPARE_ERROR, /* Flash Data Compare error */
};

enum SimpleSD_LEDModes
{
	SIMPLESD_LED_STOPPED_MODE   = 0,   /* LED is turned off */
	SIMPLESD_LED_0_5_HZ_MODE  	= 1,   /* LED blinking with 0.5Hz frequency */
	SIMPLESD_LED_1HZ_MODE  		= 2,   /* LED blinking with 1Hz frequency */
	SIMPLESD_LED_2HZ_MODE  		= 4,   /* LED blinking with 2Hz frequency */
	SIMPLESD_LED_4HZ_MODE  		= 8,   /* LED blinking with 3Hz frequency */
	SIMPLESD_LED_10HZ_MODE		= 20,  /* LED blinking with 10Hz frequency */
	SIMPLESD_LED_ON_MODE   		= 100,  /* LED blinking with 50Hz frequency */
};

enum SimpleSD_Detect
{
	SIMPLESD_NOT_DETECTED = 0,   /* SD card has NOT been detected */
	SIMPLESD_DETECTED 	  = 1,	 /* SD card has been detected */
};

enum SimpleSD_CRC
{
	SIMPLESD_CRC_SAME    = 0,     /* Calculated CRC and stored CRC are same */
	SIMPLESD_CRC_ERROR   = 1,	 /* Calculated CRC and stored CRC are different */
};

uint8_t SimpleSD_FirmwareUpgrade(void);
uint32_t SimpleSD_FindSector(uint32_t Address);
uint8_t SimpleSD_DetectCard(void);
void SimpleSD_JumpToMainFirmware(void);
void SimpleSD_BlinkLED(void);
void SimpleSD_DeInit(void);
void SimpleSD_ModeLED(uint8_t Mode);
uint8_t SimpleSD_CRC_Check(void);
uint32_t CalculateCRC_32(uint32_t crc, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif /* __SIMPLESD_BOOTLOADER_H */
