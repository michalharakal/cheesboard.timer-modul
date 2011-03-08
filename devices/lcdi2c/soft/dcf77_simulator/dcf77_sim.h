typedef uint8_t(*tuiCallback)(void);
	

class DCF77Simulator
{
  public:
    DCF77Simulator(int pin);
    void set_time(uint8_t hour, uint8_t minutes);
    void set_date(uint8_t year, uint8_t month, uint8_t day, uint8_t dow);
	void register_uiCallback(uint8_t(*uiCallback)(void));
	void get_time(uint8_t *hour, uint8_t *minutes);
	uint8_t get_secs(void);
    void get_date(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *dow);	
    void simulate(void);
    void IncMinute(void);	
	void reset(void);	
    void TestMuster(void);
    int bits_send;
	void set_turbo_mode(uint8_t turbo_mode);
	uint8_t get_turbo_mode(void);
  private:
    int _pin;
	uint8_t _turbo_mode;
    uint8_t _hour, _minutes, _secs;
    uint8_t _year, _month, _day, _dow;
	tuiCallback _uiCallbackFnc;
	uint8_t sendone(void);
	uint8_t sendzero(void);	
};

