#include <avr/interrupt.h>
#include "bootloader.h"

uint8_t bootloading ATTR_NO_INIT;

uint8_t mode = 0, mode_state = 0;

inline void mode_reset() {
  mode = 0;

  // Reset and passive uart pipes
  uart_pipe_passive(0);
}

inline void mode_forward_reset() {
  uart_send(mode);
  mode_reset();
}

void boot_program_page (uint32_t page, uint8_t *buf)
{
  uint16_t i;
  uint8_t sreg;

  // Disable interrupts.

  sreg = SREG;
  
  eeprom_busy_wait ();

  boot_page_erase (page);
  boot_spm_busy_wait ();      // Wait until the memory is erased.

  for (i=0; i<SPM_PAGESIZE; i+=2)
  {
    // Set up little-endian word.

    uint16_t w = *buf++;
    w += (*buf++) << 8;
    
    boot_page_fill (page + i, w);
  }

  boot_page_write (page);     // Store buffer in flash page.
  boot_spm_busy_wait();       // Wait until the memory is written.

  // Reenable RWW-section again. We need this if we want to jump back
  // to the application after bootloading.

  boot_rww_enable ();

  // Re-enable interrupts (if they were ever enabled).

  SREG = sreg;
}


void bootloader() {
  uint32_t i, length, tmp;
  uint8_t buffer[SPM_PAGESIZE];
  
  while (bootloading) {
    mode = uart_recv();
    
    while(mode) {
      switch (mode) {

        case protocol_forward:
          uart_send(protocol_forward);
          length = uart_recv16(); // collect the length
          uart_send16(length);
          uart_pipe_active(length);

          mode_reset();
        break;

        case protocol_device_type:
          // setup and emit a forward packet
          // containing 2 bytes of the device
          // signature.
        
          uart_send(protocol_forward);
          uart_send16(0x0003);
          uart_send(protocol_device_type);
          uart_send32(DEVICE_SIGNATURE);
          
          mode_forward_reset();
        break;
        
        case protocol_program:
          uart_send(mode);        
          uart_pipe_passive(8);

          // read the device signature, it is important that we don't
          // attempt to flash this chip with another device type's firmware
          tmp = uart_recv32();

          // read the length of the data bytes (uint32_t)
          length = uart_recv32();
          uart_pipe_passive(length);
          
          if (tmp == DEVICE_SIGNATURE) {
            uint8_t buffer_loc = 0;
            uint32_t program_page_loc = 0;
            for (i = 0; i<length; i++) {
              buffer[buffer_loc++] = uart_recv();
              if (buffer_loc >= SPM_PAGESIZE) {
                boot_program_page(program_page_loc, buffer);
                program_page_loc += SPM_PAGESIZE;
                buffer_loc = 0;
                memset(buffer, 0xFF, SPM_PAGESIZE);
              }
            }
              
            boot_program_page(program_page_loc, buffer);

            bootloading = 0;
            
          } else {
            // if this device does not match the incoming device
            // signature, then just pass it on.
            uart_pipe_active(length);
          }

          mode_reset();
        break;
        
        case protocol_read_flash:

          uart_send(protocol_forward);
          uart_send16(BOOT_START_ADDR+1);
          uart_send(protocol_read_flash);
          
          for (i = 0; i < BOOT_START_ADDR; i+=2) {
            uart_send16LE(pgm_read_word(i));
          }

          mode_forward_reset();
        break;
        
        case protocol_erase:
          for (i = 0; i < BOOT_START_ADDR; i += SPM_PAGESIZE) {
            boot_page_erase(i);
            boot_spm_busy_wait();
          }
  
          mode_forward_reset();
        break;

        case protocol_reset:
          mode_forward_reset();
        break;

        default:
          uart_send(protocol_forward);
          uart_send(protocol_error);
          uart_send(mode);
          uart_send16(0x0404);
          mode_reset();
        break;
      }
    }
  }
}

int main(void) {

  /* Disable watchdog if enabled by bootloader/fuses */
  uint8_t was_watchdog_reset = MCUSR & (1 << WDRF);
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  /* Disable clock division */
  clock_prescale_set(clock_div_1);

  uart_sync_init();

  // if the flash has not been written, successfully,
  // by this bootloader.
  if (pgm_read_word(0) == 0xFFFF || was_watchdog_reset) {
    bootloading = 1;
  }

  if (bootloading) {
    bootloader();
  }
  
  // Execute the program
  ((void (*)(void))0x0000)();
  
  // Now we enable the watchdog timer and wait for reset.
  wdt_enable(WDTO_250MS);
  for (;;);
}
