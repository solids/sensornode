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
  
  /* Disable clock division */
  clock_prescale_set(clock_div_1);
  
  uart_sync_init();
  while(1) {
    uart_pipe_passive(1);

    if (uart_recv() == protocol_reset) {
      wdt_enable(WDTO_1S);
      for (;;);
    }
  }
}