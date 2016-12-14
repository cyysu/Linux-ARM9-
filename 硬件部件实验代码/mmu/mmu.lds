SECTIONS { 
  firtst    0x00000000 : { head.o init.o }
  second    0xB0004000 : AT(2048) { leds.o }
} 
 