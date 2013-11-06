#include "uart_sync.h"

void uart_send(char c) {
  while (( UCSR0A & (1 << UDRE0 )) == 0) {};
  UDR0 = c;
}

void uart_send16(uint16_t c) {
  uart_send(c >> 8);
  uart_send(c & 0x00FF);
}

void uart_send16LE(uint16_t c) {
  uart_send(c & 0x00FF);
  uart_send(c >> 8);
}

void uart_send32(uint32_t c) {
  char *p = (char *)&c;
  uart_send(p[0]);
  uart_send(p[1]);
  uart_send(p[2]);
  uart_send(p[3]);
}

uint8_t uart_recv_pending() {
  return ((UCSR0A & (1 << RXC0 )));
}

uint32_t uart_proxy_bytes_remaining = 0;
uint8_t uart_recv() {
  while (( UCSR0A & (1 << RXC0 )) == 0) {}; // Do nothing until data have been received and is ready to be read from UDR

  uint8_t c = UDR0;

  if (uart_proxy_bytes_remaining) {
    uart_proxy_bytes_remaining--;
    uart_send(c);
  }

  return c;
}

uint16_t uart_recv16() {
  return (uart_recv() << 8 | uart_recv());
}

uint16_t uart_recv16LE() {
  return uart_recv() | (uart_recv() << 8);
}

uint32_t uart_recv32() {
  return (
    (uint32_t)uart_recv() << 24 |
    (uint32_t)uart_recv() << 16 |
    (uint16_t)uart_recv() << 8  |
    uart_recv()
   );
}

void uart_pipe_passive(uint32_t length) {
  uart_proxy_bytes_remaining = length;
}

void uart_pipe_active(uint32_t length) {
  while (length--) {
    uart_send(uart_recv());
  }
}

void uart_sync_init() {

  // Turn on the transmission and reception circuitry
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  
  // Use 8- bit character sizes
  UCSR0C = (1 << UMSEL01 ) | (1 << UCSZ00) | (1 << UCSZ01); 
  
  // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
  UBRR0H = (BAUD_PRESCALE >> 8); 
  
  // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register  
  UBRR0L = BAUD_PRESCALE; 
}
