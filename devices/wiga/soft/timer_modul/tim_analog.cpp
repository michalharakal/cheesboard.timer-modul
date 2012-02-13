#include <stdio.h>

#include "tim_analog.h"
#include "../common/caux.h"
#include "../common/dcf.h"

void TimedAnalog::SetChannel(uint8_t channel, uint8_t inverted)
{
    _channel = channel;
    _inverted = inverted;
    _last_value_high = IsSignalHigh ();
}

void TimedAnalog::SetTreshold(uint16_t treshold, uint32_t treshold_low, uint32_t treshold_high)
{
    _treshold = treshold;
    _treshold_high = treshold_high;
    _treshold_low = treshold_low;
    _counter_high = 0;
    _counter_low = 0;
    _wert_highDa = 0;
    _wert_lowDa = 0;
    AnalogSignal();
}

uint8_t TimedAnalog::IsSignalHigh (void)
{
    uint8_t result = ReadAD (_channel, 0) >= _treshold;

    // inverted?
    if (_inverted)
        result = !result;

    return result;
}

void TimedAnalog::Clean(void) {
    _wert_highDa = 0;
    _wert_lowDa = 0;
}

uint8_t TimedAnalog::LowDa(void) {
    return  _wert_lowDa;
}

uint8_t TimedAnalog::HighDa(void) {
    return  _wert_highDa;
}


void TimedAnalog::AnalogSignal()
{
    // analog Wert einlesen und mit Schwele vergleichen
    if (IsSignalHigh ()) {
        // was vorher auch high?
        if (_last_value_high) {
            // Zeitzähler um eine Sekunde erhöhen, wenn die Grenze noch nicht erreicht ist
            if (_counter_high < _treshold_high)
                _counter_high++;
        }
        else {			// schwelle von unten nach oben überschritten

            // beide Zeitzähler löschen
            _counter_low = 0;
            _counter_high = 0;
            _wert_lowDa = 0;
            _wert_highDa = 0;
#ifdef DEBUG
            printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
            printf("Analog ch=%d reset after treshcross from low\r\n", _channel);
#endif
        }
        // signal für mehr als definerte Zeit da?
        if (_counter_high >= _treshold_high) {
            if (_wert_highDa == 0) {
#ifdef DEBUG
                printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
                printf("Analog ch=%d highDa\r\n", _channel);
#endif
            }
            _wert_highDa = 1;
        }
        // aktueler Zustand merken
        _last_value_high = 1;
    }
    else {			// aktueller Analogwert ist unter dem Schwelenwert
        if (_last_value_high) {	// Schwelle von oben nach unten überschritten
            // beide Zeitzähler löschen
            _counter_low = 0;
            _counter_high = 0;
            _wert_lowDa = 0;
            _wert_highDa = 0;
#ifdef DEBUG
            printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
            printf("Analog ch=%d reset after treshcross from high\r\n", _channel);
#endif
        }
        else {
            if (_counter_low < _treshold_low)
                _counter_low++;
        }
        // kein Signal mehr
        if (_counter_low >= _treshold_low) {
            if (_wert_lowDa == 0) {
#ifdef DEBUG
                printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
                printf("Analog ch=%d _wert_lowDa = %d\r\n", _channel, 1);
#endif
            }
            _wert_lowDa = 1;
        }
        // aktueler Zustand merken
        _last_value_high = 0;
    }
}
