// Sonderversion basierend auf Stand vom 27.07.09
// letzte �nderung am:
// 30.09.2009 
// 18.05.2010
// 17.07.2010
// 12.12.2010 Rolladen runter 10min --> 30min nach Abendlicht
// 07.03.2011 Rolladen runter 30min --> 20min nach Abendlicht

// TO DO 
//  - LEDs auf neue Definition anpassen und aktualisieren (zuerst Docs PK)
//  - DCF77 emfp�ngert auf 50 Miliseconds f�r "0"

#ifndef F_CPU
/* wenn F_CPU nicht bereits vorher definiert (z.B. durch
   �bergabe als Parameter zum Compiler innerhalb des Makefiles) */
#define F_CPU 14745600UL     /* Quarz mit 14.7456 Mhz */
#endif


#include <stdio.h>
#include <util/twi.h> 	    //enth�lt z.B. die Bezeichnungen f�r die Statuscodes in TWSR
#include <avr/interrupt.h>  //dient zur Behandlung der Interrupts
#include <stdint.h> 	    //definiert den Datentyp uint8_t
#include "twislave.h"
#include <stdlib.h>         //n�tig f�r Zahlumwandlung mit itoa
#include <util/delay.h>


#include "gd_driver.h"
#include "graphics.h"
#include "widgets.h"
#include "keyboard.h"
#include "menus.h"
#include "status.h"

#define SLAVE_ADRESSE 0x50 //Die Slave-Adresse

/**
 * updates display
 */ 
void ReceivingTime(void) 
{   
  gd_hw_clear_scr();    
  Bar(0,56,127,64);
  ShowStr(2, 55, "Abendlicht 2011", -1);  
  ShowStr(2, 26, "Zeitempfang ... ", -1);    
  gd_hw_update();	  
}

void UpdateDisplay(void) 
{   
    gd_hw_clear_scr();    
    Bar(0,56,127,64);
    gd_hw_update();	  
}

void update_clock(void)
{
  gd_hw_clear_scr(); 
  char auxstr[30];
  sprintf(auxstr, "%.2d:%.2d:%.2d", i2cdata[0], i2cdata[1], i2cdata[2]);
  ShowStr(10, 10, auxstr, -1);
  gd_hw_update();	 
}  


int main (void)
{   
  //TWI als Slave mit Adresse slaveadr starten
  init_twi_slave(SLAVE_ADRESSE);

  // i2cdatamit Werten f�llen, die der Master auslesen und �ndern kann
  for(uint8_t i=0;i<i2c_buffer_size;i++)
  {
		i2cdata[i]=48+i;
  }


// graphics driver init
  gInit();	
  // backlight on
  gd_hw_backlight(1);   
  
  ReceivingTime();
  
  update_clock();
  
// check data on bus
  for(;;) {
    if (new_data_here) {
    
      new_data_here = 0;
      update_clock();
    }
  	  //CheckI2c();
  }	      
}
