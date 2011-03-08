class DCF77Simulator
{
  public:
    DCF77Simulator(int pin);
    void set_time(uint8_t hour, uint8_t minutes, uint8_t seconds);
    void set_date(uint8_t year, uint8_t month, uint8_t day, uint8_t dow);
  private:
    int _pin;
    uint8_t _hour, _minutes, _seconds;
    uint8_t _year, _month, _day, _dow;
};

