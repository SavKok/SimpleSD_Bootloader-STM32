/*
  ******************************************************************************
  * @file    SimpleSD_bootloader.c
  * @author  Savvas Kokkinidis - sabbaskok@hotmail.com
  * @version V1.0
  * @date    14-July-2020
  * @brief   This file contains the functions related to the
  *          the SimpleSD_bootloader
  ******************************************************************************
  * @Copyright 2020 Savvas Kokkinidis
  ******************************************************************************
  */

#include "main.h"
#include "SimpleSD_bootloader.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "fatfs.h"

#if CRC_CALCULATION_METHOD
#include "stm32f4xx_hal_crc.h"
extern CRC_HandleTypeDef hcrc;
#endif

#if SD_WATCHDOG_RUNNING
extern IWDG_HandleTypeDef hiwdg;
#endif

static FATFS FileSystem;   // FileSystem
static FIL SimpleSD_file;  // SD file
static FRESULT fresult;    // Result

static int16_t LED_Reload_Value;
static int16_t LED_Toggle_Counter;

typedef  void (*pFunction)(void);

/*
 * @brief  Firmware upgrade from SD
 * @param  None
 * @retval enum SimpleSD_ErrorCodes:
* 					- SIMPLESD_OK       			 	  Success
*					- SIMPLESD_NO_SD:	 				  No SD detected
*					- SIMPLESD_FS_MOUNT_ERROR:	 		  Mount error
*					- SIMPLESD_FS_OPEN_ERROR:		 	  FS Open error
*					- SIMPLESD_FS_READ_ERROR:		 	  FS Read error
*					- SIMPLESD_FLASH_ERASE_ERROR:	 	  Flash Erase error
*					- SIMPLESD_FLASH_WRITE_ERROR:	 	  Flash Write error
*					- SIMPLESD_FLASH_WRITE_COMPARE_ERROR: Flash Data Compare error
*/

uint8_t SimpleSD_FirmwareUpgrade(void)
{
	  UINT Bytes;
	  uint32_t FileData;
	  uint32_t firstSector, max_sectors, Address, sectorError;
	  FLASH_EraseInitTypeDef EraseStruct;

	  /* Turn off LED */
	  SimpleSD_ModeLED(SIMPLESD_LED_STOPPED_MODE);

#if SD_WATCHDOG_RUNNING
	  HAL_IWDG_Refresh(&hiwdg);
#endif

	  if(SimpleSD_DetectCard())
	  {
		  fresult = f_mount(&FileSystem,APPLICATION_FS_DIR, 1);
		  if(fresult != FR_OK) {
			  return SIMPLESD_FS_MOUNT_ERROR;
		  }

		  /* Open file with read access */
		  fresult = f_open(&SimpleSD_file,APPLICATION_BIN_FILENAME, FA_OPEN_EXISTING | FA_READ);
		  if(fresult != FR_OK) {
			  /* De-initialization of SD-FileSystem */
			  SimpleSD_DeInit();
			  return SIMPLESD_FS_OPEN_ERROR;
		  }

		  /* Toggle LED with 4Hz frequency*/
		  SimpleSD_ModeLED(SIMPLESD_LED_4HZ_MODE);

#if SD_WATCHDOG_RUNNING
		  HAL_IWDG_Refresh(&hiwdg);
#endif

		  /* Unlock the Flash to enable the flash control register access *************/
		  HAL_FLASH_Unlock();

		  /* Clear Flash error flags flag */
		  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR | FLASH_FLAG_BSY);

		  /* Get the 1st sector to erase */
		  firstSector = SimpleSD_FindSector(APPLICATION_START_ADDRESS);

		  /* Get the number of sectors to erase from 1st sector*/
		  max_sectors = SimpleSD_FindSector(APPLICATION_END_ADDRESS) - firstSector + 1;

		  /* Fill EraseInit structure First Half*/
		  EraseStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
		  EraseStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
		  EraseStruct.NbSectors = 1;

		  for(uint32_t current_sector = firstSector; current_sector < (firstSector + max_sectors); current_sector++) {
			  EraseStruct.Sector = current_sector;
			  if(HAL_FLASHEx_Erase(&EraseStruct, &sectorError) != HAL_OK) {
				  /* De-initialization of SD-FileSystem */
				  SimpleSD_DeInit();
				  /* Locks the FLASH control register access. */
				  HAL_FLASH_Lock();
				  /* Flash Erase error */
				  return SIMPLESD_FLASH_ERASE_ERROR;
			  }
		  }

		  /* Clear Flash error flags flag */
		  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR | FLASH_FLAG_BSY);


#if SD_WATCHDOG_RUNNING
		  HAL_IWDG_Refresh(&hiwdg);
#endif

#if SD_WATCHDOG_RUNNING
		  HAL_IWDG_Refresh(&hiwdg);
#endif

		  /* Disable the FLASH data cache */
		  __HAL_FLASH_DATA_CACHE_DISABLE();
		  /* Disable the FLASH instruction cache */
		  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
		  /* Resets the FLASH data Cache. */
		  __HAL_FLASH_DATA_CACHE_RESET();
		  /* Resets the FLASH instruction Cache. */
		  __HAL_FLASH_INSTRUCTION_CACHE_RESET();
		  /* Enable the FLASH instruction cache */
		  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
		  /* Enable the FLASH data cache. */
		  __HAL_FLASH_DATA_CACHE_ENABLE();

#if SD_WATCHDOG_RUNNING
		  HAL_IWDG_Refresh(&hiwdg);
#endif

		  /* Toggle LED with 10Hz frequency*/
		  SimpleSD_ModeLED(SIMPLESD_LED_10HZ_MODE);

		  /* Clear Flash error flags flag */
		  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_RDERR | FLASH_FLAG_BSY);


		  Address = APPLICATION_START_ADDRESS;
		  while (Address < APPLICATION_END_ADDRESS)
		  {
			fresult = f_read(&SimpleSD_file, &FileData, sizeof(FileData), &Bytes);
			if((Bytes == 0) || (fresult != FR_OK)) {
				/* De-initialization of SD-FileSystem */
				SimpleSD_DeInit();
				/* Locks the FLASH control register access. */
				HAL_FLASH_Lock();
				/* FS Read error */
				return SIMPLESD_FS_READ_ERROR;
			}

			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, FileData) == HAL_OK)	{
			  if(*(uint32_t*)Address != FileData) {
				 /* De-initialization of SD-FileSystem */
				 SimpleSD_DeInit();
				 /* Locks the FLASH control register access. */
				 HAL_FLASH_Lock();
				 /* Flash Data Compare error */
			     return SIMPLESD_FLASH_WRITE_COMPARE_ERROR;
			  }
			  Address += sizeof(FileData);
			}
			else {
				/* De-initialization of SD-FileSystem */
				SimpleSD_DeInit();
				/* Locks the FLASH control register access. */
				HAL_FLASH_Lock();
				/* Flash Write error */
				return SIMPLESD_FLASH_WRITE_ERROR;
			}
		  }
		  /* De-initialization of SD-FileSystem */
		  SimpleSD_DeInit();
		  /* Locks the FLASH control register access. */
		  HAL_FLASH_Lock();
		  /* Success */
		  if(SimpleSD_CRC_Check() == SIMPLESD_CRC_SAME) {
			  return SIMPLESD_OK;
		  }
	  }
	  /* No SD detected */
	  return SIMPLESD_NO_SD;
}

/*
 * @brief  Finds the desired Flash sector based on the input address
 * @param  Address: The desired address on flash
 * 					- APPLICATION_START_ADDRESS to APPLICATION_END_ADDRESS: Any address on the area of main application
 * @retval sector:
 * 					- 0 to 23: The flash sector
 */
uint32_t SimpleSD_FindSector(uint32_t Address)
{
  uint32_t sector = 0;

  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0)) {
    sector = FLASH_SECTOR_0;
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1)) {
    sector = FLASH_SECTOR_1;
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2)) {
    sector = FLASH_SECTOR_2;
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3)) {
    sector = FLASH_SECTOR_3;
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4)) {
    sector = FLASH_SECTOR_4;
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5)) {
    sector = FLASH_SECTOR_5;
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6)) {
    sector = FLASH_SECTOR_6;
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7)) {
    sector = FLASH_SECTOR_7;
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8)) {
    sector = FLASH_SECTOR_8;
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9)) {
    sector = FLASH_SECTOR_9;
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10)) {
    sector = FLASH_SECTOR_10;
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11)) {
    sector = FLASH_SECTOR_11;
  }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12)) {
    sector = FLASH_SECTOR_12;
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13)) {
    sector = FLASH_SECTOR_13;
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14)) {
    sector = FLASH_SECTOR_14;
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15)) {
    sector = FLASH_SECTOR_15;
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16)) {
    sector = FLASH_SECTOR_16;
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17)) {
    sector = FLASH_SECTOR_17;
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18)) {
    sector = FLASH_SECTOR_18;
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19)) {
    sector = FLASH_SECTOR_19;
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20)) {
    sector = FLASH_SECTOR_20;
  }
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21)) {
    sector = FLASH_SECTOR_21;
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22)) {
    sector = FLASH_SECTOR_22;
  }
  else {
    sector = FLASH_SECTOR_23;
  }
  return sector;
}

/*
 * @brief  De-initialization of SD-FileSystem
 * @param  None
 * @retval None
 */
uint8_t SimpleSD_DetectCard(void)
{
	for (uint8_t counter=0; counter < SDSimple_CD_Detect_Samples; counter++) {
		if(HAL_GPIO_ReadPin(SDSimple_CD_Port, SDSimple_CD_Pin) != SDSimple_CD_Detect_Level) {
			return SIMPLESD_NOT_DETECTED;
		}
		HAL_Delay(1);
	}
	return SIMPLESD_DETECTED; // SD detected
}

/*
 * @brief  De-initialization of SD-FileSystem
 * @param  None
 * @retval None
 */
void SimpleSD_DeInit(void)
{
    f_close(&SimpleSD_file);
	f_mount(NULL, (TCHAR const*)APPLICATION_FS_DIR, 0);
}

/*
 * @brief  Jumps to the application area
 * @param  None
 * @retval None
*/
void SimpleSD_JumpToMainFirmware(void)
{
		pFunction JumpToApplication;
		uint32_t JumpAddress;

		HAL_RCC_DeInit();
		HAL_DeInit();

		SysTick->CTRL = 0;
		SysTick->LOAD = 0;
		SysTick->VAL  = 0;
		SCB->VTOR = APPLICATION_START_ADDRESS;

		JumpAddress = *(__IO uint32_t*) (APPLICATION_START_ADDRESS + 4);
		JumpToApplication = (pFunction) JumpAddress;

		/* Initialize user application's Stack Pointer */
		__set_MSP((*(__IO uint32_t*) APPLICATION_START_ADDRESS ));
		JumpToApplication();
}


/*
 * @brief  Toggle or switch off the LED based on the LED_Reload_Value and LED_Toggle_Counter
 * @param  None
 * @retval None
*/
void SimpleSD_BlinkLED(void)
{
	if(!LED_Reload_Value) {
		/* Turn off LED */
		HAL_GPIO_WritePin(SDSimple_LED_Port, SDSimple_LED_Pin, GPIO_PIN_SET);
	}
	else {
		LED_Toggle_Counter--;
		if(LED_Toggle_Counter <= 0) {
			/* Toggle LED */
			HAL_GPIO_TogglePin(SDSimple_LED_Port, SDSimple_LED_Pin);
			/* Reload counter */
			LED_Toggle_Counter = LED_Reload_Value;
		}
	}
}

/*
 * @brief  Select the blinking frequency for LED indicator
 * @param  Mode: The blinking frequency for LED indicator
 * 				- 0:  LED is turned off
 * 				- >0: LED is blinking with frequency same as Mode
 * @retval None
*/
void SimpleSD_ModeLED(uint8_t Mode)
{
	/* Set the reload value based on selected mode */
	if(Mode != SIMPLESD_LED_STOPPED_MODE) {
		LED_Reload_Value = 1000/Mode;
	}
	else {
		LED_Reload_Value = 0;
	}
}

/*
 * @brief  Check the CRC for internal Flash
 * @param  None
 * @retval enum SimpleSD_ErrorCodes:
* 					- SIMPLESD_CRC_CORRECT       		  Calculated CRC and stored CRC are same
*					- SIMPLESD_CRC_MISSMATCH:	 		  Calculated CRC and stored CRC are different
*/
uint8_t SimpleSD_CRC_Check(void)
{
	uint8_t result;
	uint32_t calculated_crc = 0xFFFFFFFF,flash_crc = 0x00000000, address, count_crc;

	/* CRC Calculation using peripheral or software:
	 * Tested on STM32F429 running on 180 MHz with 1.9MBytes bin file
	 * 			- Peripheral: ~1mSec
	 * 			- Software function: ~2450 mSec
	 */
	result = SIMPLESD_CRC_SAME;
	flash_crc = *(uint32_t*)APPLICATION_CRC_ADDRESS;
#if CRC_CALCULATION_METHOD
	calculated_crc = HAL_CRC_Calculate(&hcrc, (uint32_t *)APPLICATION_START_ADDRESS, 16);
#else
	address   = APPLICATION_START_ADDRESS;
	count_crc = APPLICATION_CRC_CALCULATION_SIZE;
	while (count_crc--)
	{
		calculated_crc = CalculateCRC_32(calculated_crc, *(uint32_t*)address);
		address += 4;
	}
#endif
	if(calculated_crc != flash_crc) {
		result = SIMPLESD_CRC_ERROR;
	}
	return result;
}


/*
 * @brief  Calculate CRC based on defined polynomial
 * @param  crc: Previous calculated CRC. The variable holds CRC should be initialized with 0xFFFFFFFF
 * 		  		- 0x00000000 to 0xFFFFFFFF
 * 		  data: Incoming data for calculation
 * 		  		- 0x00000000 to 0xFFFFFFFF
 * @retval enum SimpleSD_ErrorCodes:
* 					- SIMPLESD_CRC_CORRECT       		  Calculated CRC and stored CRC are same
*					- SIMPLESD_CRC_MISSMATCH:	 		  Calculated CRC and stored CRC are different
*/
uint32_t CalculateCRC_32(uint32_t crc, uint32_t data)
{
  crc = crc ^ data;
  for(uint8_t i=0; i < 32; i++) {
    if (crc & 0x80000000) {
    	crc = (crc << 1) ^ CRC_POLYNOMIAL;
    }
    else {
    	crc = (crc << 1);
    }
  }
  return(crc);
}
