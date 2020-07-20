# SimpleSD_Bootloader-STM32
SimpleSD_Bootloader is a simple library for firmware upgrade from SD card on STM32 microcontrollers. 

# Functions
  - Firmware upgrade based on user's definitions [application area, crc area, crc method etc]
  - CRC calculation based on peripheral or function
  - LED indicator for upgrade status  

# Hardware
  - SD card [connected to the STM32 MCU using SPI interface]
  ![hardware-img](https://github.com/SavKok/SimpleSD_Bootloader-STM32/blob/master/SD%20&%20SPI%20interface.png?raw=true)
  - LED indicator 

# Testing
Tested with STM32F429ZIT6 and 1.8MB **.bin** file, under below conditions:
  - MCU running on 180 MHz 
  - SPI clock set on 11.25 MBit/s
  - CRC calculated based on peripheral

Total time: 45 seconds 
