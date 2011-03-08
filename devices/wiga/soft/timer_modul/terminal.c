#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <avr/pgmspace.h>

#include "terminal.h"
#include "main.h"
#include "../common/uart.h"
#include "../common/dcf.h"

unsigned char AsciiToHex(unsigned char high,unsigned char low)
{
  unsigned char hex_value;

  // check if lower equal 9 ( assii 57 )
  if(high <= 57) // high is a number
    high = high -48;
  else // high is a letter
    high = high -87;

  high = high << 4;
  high = high & 0xF0;
 
  // check if lower equal 9 ( assii 57 )
  if(low <= 57) // high is a number
    low = low -48;
  else // high is a letter
    low = low -87;
  low = low & 0x0F;
 
  hex_value = high | low;
 
  return hex_value;
}

void SendHex(unsigned char hex)
{
  unsigned char high,low;
  // get highnibble
  high = hex & 0xF0;
  high = high >> 4;
 
  // get lownibble
  low = hex & 0x0F;
 
  if(high<=9)
    uart_putc(high+48);
  else
    uart_putc(high+87);
 
 
  if(low<=9)
    uart_putc(low+48);
  else
    uart_putc(low+87);
}

//this is only my debug  tool
void Terminal(char cmd)
{  
  char h,l;
  unsigned char tmp;
  
  switch(cmd)
  {   
    // write to virtual register
    case 'w':
      h = uart_getc();
      l = uart_getc();
      SendHex(AsciiToHex(h,l));
      tmp = AsciiToHex(h,l);
	  printf(PSTR("value:"));
      h = uart_getc();
      l = uart_getc();
      SendHex(AsciiToHex(h,l));
      //USBNWrite(tmp,AsciiToHex(h,l));
      uart_put(PSTR("result:"));
      SendHex(0); // to be implemented
      uart_put(PSTR("\r\n"));
    break;
    // read from virtual register
    case 't': // turbo mode
	  h = uart_getc();
      l = uart_getc();
//	  puls_factor = AsciiToHex(h,l);
      uart_put(PSTR("turbo mode:"));	  
      //SendHex(puls_factor); 
      uart_put(PSTR("\r\n"));	   
    break;
	case 'a': // analog 
	  h = uart_getc();
      l = uart_getc();
      SendHex(AsciiToHex(h,l));
      uart_put(PSTR("analog delay:"));	  
      SendHex(analog_factor); // to be implemented
      uart_put(PSTR("\r\n"));	   
    break;
    case 'h':
	  uart_put(PSTR("t dcf turbo mode <t> 10=default 1 = 10x faster"));
	  uart_put(PSTR("d debug level <l> 0=off 1=\r\n"));
      uart_put(PSTR("w write virtual Register <h,l>(address) <h,l> (value) e.g 05 00\r\n"));
      uart_put(PSTR("r read virtual Register <h,l> e.g. 02 ( RID)\r\n"));    
    break;        
    default:
      uart_put(PSTR("unknown command\r\n"));
  }
}

