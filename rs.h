#pragma once

#include "CONF.H"

namespace rs
{
    char buffer[CONF_BUFFER_SIZE];

    bool _start = false;
    int _idx = 0;

    bool command = false;
    int length = 0;

    void setup()
    {
        pinMode(PIN_RS_ENABLE, OUTPUT);

        digitalWrite(PIN_RS_ENABLE, LOW);
        delay(10);

        Serial.begin(CONF_BAUD_RATE);

        #ifdef DEBUG
        Serial.println("@DBGINF: BOOT");
        #endif
    }

    inline void send_mode()
    {
        digitalWrite(PIN_RS_ENABLE, HIGH);
        delay(10);
    }

    inline void receive_mode()
    {
        digitalWrite(PIN_RS_ENABLE, LOW);
        delay(10);
    }

    void update()
    {
        if(command)
            return;

        while(Serial.available())
        {
            auto rd = Serial.read();

            if(!_start)
            {
                if(rd == CONF_CHAR_START)
                {
                    _start = true;
                    _idx = 0;
                }
                
                continue;
            }

            if(rd == CONF_CHAR_END)
            {
                command = true;
                length = _idx;
                buffer[_idx] = '\0';
                _start = false;

                return;
            }

            buffer[_idx] = rd;

            _idx++;

            if(_idx >= CONF_BUFFER_SIZE)
            {
                _start = false;
            }
        }
    }
}