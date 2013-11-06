/*
 * uart.h
 *
 * Created: 11/3/2013 1:58:16 PM
 *  Author: tmpvar
 */ 


#ifndef UART_H_
#define UART_H_

  #include <avr/io.h>
  #include <inttypes.h>

  #ifndef F_CPU
    #define F_CPU 8000000
  #endif

  #ifndef USART_BAUDRATE
    #define USART_BAUDRATE 115200
  #endif
  
  #define BAUD_PRESCALE ((( F_CPU / ( USART_BAUDRATE * 16LU ))) - 1)

  #ifndef BOOT_START_ADDR
    // boot start byte address
    #define BOOT_START_ADDR 0x1800 // bootz is set to 0x0C00 which is in words
  #endif

  #ifndef DEVICE_SIGNATURE
    #define DEVICE_SIGNATURE 0x00000100
  #endif

#endif /* UART_H_ */