/*
 * uart_sync.h
 *
 * Created: 11/3/2013 1:54:18 PM
 *  Author: tmpvar
 */ 


#ifndef UART_SYNC_H_
#define UART_SYNC_H_

  #include "uart.h"

  void uart_send(char c);
  void uart_send16(uint16_t c);
  void uart_send16LE(uint16_t c);
  void uart_send32(uint32_t c);

  uint8_t uart_recv_pending();

  uint8_t uart_recv();
  uint16_t uart_recv16();
  uint16_t uart_recv16LE();
  uint32_t uart_recv32();

  void uart_pipe_passive(uint32_t length);
  void uart_pipe_active(uint32_t length);

  void uart_sync_init();


#endif /* UART_SYNC_H_ */