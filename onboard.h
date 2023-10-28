#pragma once

#include "CONF.H"

namespace onboard
{
    const int _LED_COUNT = 3;
    const char _LED_PINS[] = {PIN_LED1, PIN_LED2, PIN_LED3};

    int led[] = {1,1,1};
    bool led_states[] = {1,1,1};

    unsigned long led_times[] = {0,0,0};

    bool btn_last = false;
    unsigned long btn_last_change = 0;

    bool btn_changed = false;

    unsigned long next_measure = 0;
    int voltage_5 = 0;
    int voltage_12 = 0;

    void setup()
    {
        for(auto i = 0; i < _LED_COUNT; i++)
        {
            pinMode(_LED_PINS[i], OUTPUT);
            digitalWrite(_LED_PINS[i], HIGH);
        }

        pinMode(PIN_BUTTON, INPUT_PULLUP);

        analogReference(INTERNAL);

        pinMode(PIN_M5V, INPUT);
        pinMode(PIN_M12V, INPUT);
        
    }

    void update()
    {
        const auto t = millis();

        for(auto i = 0; i < _LED_COUNT; i++)
        {
            if(led[i] > 1) {
                // Blink
                auto target = led_times[i];

                if(target == 0 || t >= target)
                {
                    led_states[i] = !led_states[i];
                    led_times[i] = t + led[i];
                }
            } else if(led[i] < 0) {
                // Delayed off
                auto target = led_times[i];

                if(t >= target)
                {
                    led[i] = 0;
                    led_states[i] = 0;
                } else {
                    led_states[i] = 1;
                }
            } else {
                // Rewrite
                led_states[i] = led[i];
            }

            digitalWrite(_LED_PINS[i], led_states[i]);
        }

        const bool btn = !digitalRead(PIN_BUTTON);

        if(btn != btn_last)
        {
            if(btn_last_change == 0)
                btn_last_change = t;

            if((t - btn_last_change) > CONF_BUTTON_MIN_TIME_MS)
            {
                btn_last_change = 0;
                btn_last = btn;

                btn_changed = true;
            }
        }
        else
        {
            btn_last_change = 0;
        }

        if(next_measure < t)
        {
            next_measure = t + CONF_MEASURE_DELAY;

            voltage_5 = map(analogRead(PIN_M5V), 0, 1024, 0, CONF_M5V_MAX);
            voltage_12 = map(analogRead(PIN_M12V), 0, 1024, 0, CONF_M12V_MAX);

            // voltage_5 = analogRead(PIN_M5V);
            // voltage_12 = analogRead(PIN_M12V);

            #ifdef DEBUG_VOLTAGES
            Serial.print("@DBGINF: Voltages 5 @ ");
            Serial.print(voltage_5);
            Serial.print(", 12 @ ");
            Serial.println(voltage_12);
            #endif
        }
    }

    inline void setLedDelayed(const int i, const unsigned long time)
    {
        led[i] = -1;
        led_times[i] = millis() + time;
    }

    inline void setLedDelayedU(const int i, const unsigned long time)
    {
        setLedDelayed(i, time);
        update();
    }

    inline void setLed(const int i, const int mode)
    {
        led[i] = mode;
    }

    inline void setLedU(const int i, const int mode)
    {
        setLed(i, mode);
        update();
    }

    void setLeds(const int mode1, const int mode2, const int mode3)
    {
        led[0] = mode1 >= 0 ? mode1 : led[0];
        led[1] = mode2 >= 0 ? mode2 : led[1];
        led[2] = mode3 >= 0 ? mode3 : led[2];
    }
    
    inline void setLedsU(const int mode1, const int mode2, const int mode3)
    {
        setLeds(mode1, mode2, mode3);
        update();
    }

    inline bool btnChanged()
    {   
        const auto keep = btn_changed;

        btn_changed = false;

        return keep;
    }

    inline bool getBtn()
    {
        return btn_last;
    }
}

