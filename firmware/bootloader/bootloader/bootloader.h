/*
 * bootloader.h
 *
 * Created: 11/3/2013 1:46:27 PM
 *  Author: tmpvar
 */ 


#ifndef BOOTLOADER_H_
  #define BOOTLOADER_H_

  #include <avr/io.h>
  #include <avr/wdt.h>
  #include <avr/boot.h>
  #include <avr/power.h>
  #include <avr/eeprom.h>
  #include <string.h>
  #include <avr/pgmspace.h>
  
  #include "uart/uart_sync.h"
  #include "protocol.h"
  #define 	ATTR_NO_INIT   __attribute__ ((section (".noinit")))

#endif /* BOOTLOADER_H_ */