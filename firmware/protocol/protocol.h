#ifndef PROTOCOL_H
#define PROTOCOL_H


enum protocol {
  // Pass data forward to the brain
  // Format: [3][packet length <= 255][packet contents]
  protocol_forward  = 2,

  protocol_device_type = 6,
  
  // Programming is performed by first putting the network
  // into programming mode.  Then the bytes that should be
  // programmed are pushed into flash memory

  // format: [4][device type hi][device type lo][length hi][length lo][[data]
  protocol_program  = 3,
  protocol_read_flash = 4,
  protocol_erase = 5,
  protocol_reset = 7,
  protocol_error = 255

};


#endif /* INCFILE1_H_ */