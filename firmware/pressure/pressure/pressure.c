/*
 * pressure.c
 *
 * Created: 11/4/2013 6:33:27 PM
 *  Author: tmpvar
 */ 


#include <avr/io.h>
#include <avr/power.h>
#include <avr/wdt.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "protocol.h"
#include "uart/uart_sync.h"
int main(void) {

  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz

  ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
  ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading
  
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);
  ADCSRA |= (1 << ADATE);
 
  
  /* Disable clock division */
  clock_prescale_set(clock_div_1);
  
  #define CYCLES_PER_SENSOR 100
  uint8_t sensor = 0, cycles = CYCLES_PER_SENSOR;
  uint16_t val = 0;
  
  
  uart_sync_init();
  while(1) {
    if (uart_recv_pending()) {
      if (UDR0 == protocol_reset) {
        uart_send(protocol_reset);
        wdt_enable(WDTO_1S);
        for (;;);
      }
    }
    
    val += ADCH;
    
    cycles--;
    if (!cycles) {

      uart_send(protocol_forward);
      uart_send16(0x0002);
      uart_send(sensor);
      uart_send((uint8_t)(val/CYCLES_PER_SENSOR));
      val = 0;
      sensor++;
      if (sensor > 6) {
        sensor = 0;
        ADMUX &= 0xF0;
        ADMUX |= sensor;
      }
      
      cycles = CYCLES_PER_SENSOR;
    }
    _delay_ms(1);
  }
}