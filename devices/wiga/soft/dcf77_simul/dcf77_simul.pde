// based on http://www.captain.at/rtai-dcf77-simulator.php
int pin = 13;
uint16_t now_min, now_hour, now_day, now_wday, now_month, now_year;

void setup()
{
  pinMode(pin, OUTPUT);
}

void sendone(void) {
  uint8_t t;

  digitalWrite(pin, HIGH);   // sets the LED on
  delay(200);  //  1 = 200 ms
  digitalWrite(pin, LOW);    // sets the LED off
  for (t = 1; t <= 8; t++) { 
    delay(100); 
  }
}

void sendzero(void) {
  uint8_t t;

  digitalWrite(pin, HIGH);   // sets the LED on
  delay(100);  //  0 = 100 ms
  digitalWrite(pin, LOW);    // sets the LED off
  for (t = 1; t <= 9; t++) { 
    delay(100); 
  }
}

uint16_t Convert2PacketBCD(uint16_t value) {
  uint16_t result;
  result = value % 10;
  result += (value - result) / 10 * 0x10;
  return result;
}

void dcf77_generator(void) {
  uint16_t i, min, hour, day, wday, month, year;
  uint16_t parity_min, parity_hour, parity_date;

  min = Convert2PacketBCD(now_min);
  hour = Convert2PacketBCD(now_hour);
  day = Convert2PacketBCD(now_day);
  wday = Convert2PacketBCD(now_wday);
  month = Convert2PacketBCD(now_month);
  year = Convert2PacketBCD(now_year);


  // generate 20 zeros
  //		for (i = 1; i <= 20; i++ ) {
  //			sendzero();
  //		}
  // send one start bit
  sendone();
  // minute
  parity_min = 0;
  for (i = 1; i <= 7; i++) {
    if (min & 1) {
      sendone();
      parity_min = parity_min ^ 1;
    } 
    else sendzero();
    min = min >> 1;
  }
  // minute_parity
  if (parity_min) { 
    sendone(); 
  } 
  else { 
    sendzero(); 
  }
  // hour
  parity_hour = 0;
  for (i = 1; i <= 6; i++) {
    if (hour & 1) {
      sendone();
      parity_hour = parity_hour ^ 1;
    } 
    else sendzero();
    hour = hour >> 1;
  }
  // hour_parity
  if (parity_hour) { 
    sendone(); 
  } 
  else { 
    sendzero(); 
  }
  parity_date = 0;
  // day
  for (i = 1; i <= 6; i++) {
    if (day & 1) {
      sendone();
      parity_date = parity_date ^ 1;
    } 
    else sendzero();
    day = day >> 1;
  }
  // weekday
  for (i = 1; i <= 3; i++) {
    if (wday & 1) {
      sendone();
      parity_date = parity_date ^ 1;
    } 
    else sendzero();
    wday = wday >> 1;
  }
  // month
  for (i = 1; i <= 5; i++) {
    if (month & 1) {
      sendone();
      parity_date = parity_date ^ 1;
    } 
    else sendzero();
    month = month >> 1;
  }
  // year
  for (i = 1; i <= 8; i++) {
    if (year & 1) {
      sendone();
      parity_date = parity_date ^ 1;
    } 
    else sendzero();
    year = year >> 1;
  }
  // date_parity
  if (parity_date) { 
    sendone(); 
  } 
  else { 
    sendzero(); 
  }
  // wait a second
  for (i = 0; i < 10; i++) { 
    delay(100);
  }
}

void IncMinute(void)
{
  // increment minute
  now_min++;
  if (now_min > 59) {
    now_min = 0;
    now_hour++;
    if (now_hour > 23) {
      now_hour = 0;
      now_day++;
      // etc...
    }
  }
}  


void loop()
{

}
