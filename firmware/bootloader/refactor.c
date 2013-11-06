uint8_t sreg;
void handle_protocol_program_init() {
}

void handle_protocol_erase() {
  for (uint32_t addr = 0; addr < (uint32_t)BOOT_START_ADDR; addr += SPM_PAGESIZE) {
    for (uint8_t i = 0; i<SPM_PAGESIZE; i+=2) {
      for(uint16_t w = 0; w < SPM_PAGESIZE; w+=2) {
        boot_page_fill(w, (uint16_t)0xEEEE);
      }
      
      boot_page_erase(addr);
      boot_spm_busy_wait();

      boot_page_write(addr);
      boot_spm_busy_wait();
    }
  }
  
  mode = 0;
  
  transmit(protocol_erase);
}

uint8_t program_page_length = 0;
uint8_t program_address[2];
uint8_t program_page_where = 0;
uint16_t program_page_base = 0;
uint32_t program_page_word;
uint8_t program_buffer[SPM_PAGESIZE];

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


uint32_t program_where = 0;
uint16_t program_length = 0;
uint32_t program_page_location = 0;
uint8_t program_buffer_loc = 0;
void handle_protocol_program(uint8_t b) {
  if (program_where < 2) {
    program_length += (program_where == 0) ? b << 8 : b;
    } else {
    program_buffer[program_buffer_loc] = b;
    program_buffer_loc++;
    
    uint8_t done = program_buffer_loc + program_page_location >= program_length;
    
    if (program_buffer_loc >= SPM_PAGESIZE || done) {
      boot_program_page(program_page_location, program_buffer);
      program_buffer_loc = 0;
      program_page_location+=SPM_PAGESIZE;
      memset(program_buffer, 0xFF, SPM_PAGESIZE);
    }
    
    if (done) {
      mode = 0;
      program_where = 0;
      program_length = 0;
    }
  }

  program_where++;
}

uint8_t forward_bytes = 0;
void handle_protocol_forward(uint8_t b) {
  if (!forward_bytes) {
    forward_bytes = b;
    } else {
    forward_bytes--;
    if (forward_bytes == 0) {
      mode = 0;
    }
  }
  
  out[0] = b;
  transfer_length = 1;
}

void handle_protocol_read_flash() {
  uint32_t i, addr, word;

  out[0] = protocol_forward;
  out[1] = 2;
  
  for (addr = 0; addr < (uint32_t)BOOT_START_ADDR; addr += SPM_PAGESIZE) {
    for (i=0; i<SPM_PAGESIZE; i+=2) {
      word = pgm_read_word(addr + i);
      
      out[3] = word >> 8;
      out[2] = word & 0x00ff;
      transfer_length = 4;
      uart_flush();
    }
  }

  mode = 0;

  out[0] = protocol_read_flash;
  transfer_length = 1;
  uart_flush();
}
