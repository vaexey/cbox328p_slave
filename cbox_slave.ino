#define FIRMWARE_VERSION_RAW "1.0.0"
// #define DEBUG
// #define DEBUG_VOLTAGES
#define USE_EEPROM

#include "CONF.H"
#include "onboard.h"
#include "timer.h"
#include "rs.h"
#include "frame.h"

#include "util.h"

#ifdef USE_EEPROM
#include <EEPROM.h>

EE_CONF eec;
#endif

void setup()
{
    eeprom_read();

    onboard::setup();
    onboard::setLedsU(1, 1, 1);

    timer::setup();
    timer::set(0, 0, 0);

    rs::setup();

    #ifdef DEBUG
    rs::send_mode();
    #endif

    delay(1000);

    onboard::setLedsU(1, 0, 150);

    frame::set_frame(0, 0,0,0);
    frame::set_frame(1, 128,128,128);
    frame::set_frame(2, 255, 0, 0);
    frame::set_frame(3, 0, 255, 0);
    frame::set_frame(4, 0, 0, 255);
}

#ifdef USE_EEPROM
void eeprom_read()
{
    EEPROM.get(CONF_EEPROM_ADDRESS, eec);

    ID_SINGLE = eec.id_single;
    APPLICABLE_IDS[0] = ID_SINGLE;
    APPLICABLE_IDS[2] = eec.id_group_1;
    APPLICABLE_IDS[3] = eec.id_group_2;

    CONF_M5V_MAX = eec.max_5v;
    CONF_M12V_MAX = eec.max_12v;
}
#endif

void loop()
{
    rs::update();

    if(rs::command)
    {
        rs::command = false;

        command();
    }

    onboard::update();

    if(onboard::btnChanged() && onboard::getBtn())
    {
        frame::set_pwm(!frame::current);

        onboard::setLedDelayed(1, 6);
    }
}

#ifdef DEBUG
void dbg_nel(int c, int len)
{
    Serial.print("@DBGERR: Cmd ");
    Serial.print(c);
    Serial.print(" n.e.l. of ");
    Serial.println(len);
}
#endif

void command()
{
    const auto len = rs::length;
    const auto c = rs::buffer[0];

    if(len < 4)
    {
        #ifdef DEBUG
        dbg_nel(c, len);
        #endif
        
        return;
    }

    auto id = str2int(rs::buffer, 1, 3);

    bool valid = false;

    for(auto i = 0; i < 3; i++)
    {
        if(APPLICABLE_IDS[i] == id)
        {
            valid = true;
            break;
        }
    }

    if(!valid)
        return;

    if(c == CONF_CMD_DIRECT)
    {
        // !1<ID><O1><O2><O3>#
        if(len < 13)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }

        auto r = str2int(rs::buffer, 4, 6);
        auto g = str2int(rs::buffer, 7, 9);
        auto b = str2int(rs::buffer, 10, 12);
        
        timer::set(r,g,b);

        onboard::setLedDelayed(1, 6);

        #ifdef DEBUG
        Serial.print("@DBGINF: Set led to ");
        Serial.print(r);
        Serial.print(", ");
        Serial.print(g);
        Serial.print(", ");
        Serial.print(b);
        Serial.print(" using id ");
        Serial.println(id);
        #endif
    } 
    
    else if(c == CONF_CMD_FRAME_HW) {
        // !2<ID><F_ID>#
        if(len < 7)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }

        auto fidx = str2int(rs::buffer, 4, 6);

        frame::set_pwm(fidx);

        onboard::setLedDelayed(1, 6);

        #ifdef DEBUG
        Serial.print("@DBGINF: Set led to ");
        Serial.print(OCR1A);
        Serial.print(", ");
        Serial.print(OCR1B);
        Serial.print(", ");
        Serial.print(OCR2A);
        Serial.print(" from frame ");
        Serial.println(fidx);
        #endif
    } 
    
    else if(c == CONF_CMD_FRAME_L) {
        // !3<ID><O1><O2><O3><F_ID>#
        if(len < 16)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }

        auto r = str2int(rs::buffer, 4, 6);
        auto g = str2int(rs::buffer, 7, 9);
        auto b = str2int(rs::buffer, 10, 12);
        auto fidx = str2int(rs::buffer, 13, 15);

        frame::set_frame(fidx, r, g, b);

        onboard::setLedDelayed(1, 6);
    } 
    
    else if(c == CONF_CMD_FRAME_HWL) {
        // !4<ID><O1><O2><O3><F_ID>#
        if(len < 16)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }

        auto r = str2int(rs::buffer, 4, 6);
        auto g = str2int(rs::buffer, 7, 9);
        auto b = str2int(rs::buffer, 10, 12);
        auto fidx = str2int(rs::buffer, 13, 15);

        frame::set_frame(fidx, r, g, b);
        frame::set_pwm(fidx);

        onboard::setLedDelayed(1, 6);
    }

    else if(c == CONF_CMD_RAPORT) {
        // !8<ID>#

        rs::send_mode();
        delay(10);

        Serial.print("@R");
        Serial.print(ID_SINGLE);
        Serial.print(";");
        Serial.print(FIRMWARE_VERSION);
        Serial.print(";");
        Serial.print(onboard::voltage_5);
        Serial.print(";");
        Serial.print(onboard::voltage_12);
        // Serial.print(";");
        Serial.print("$");
        Serial.flush();

        delay(15);
        rs::receive_mode();
        // delay(10);

        onboard::setLedDelayed(1, 6);
    }

    #ifdef USE_EEPROM
    else if(c == CONF_CMD_CALIB_5) {
        // !C<ID><MAX>#
        if(len < 9)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }
        
        auto v5 = str2int(rs::buffer, 4, 8);

        auto rd = analogRead(PIN_M5V);

        auto calibMax = map(1024, 0, rd, 0, v5);

        eec.max_5v = calibMax;
        EEPROM.put(CONF_EEPROM_ADDRESS, eec);

        onboard::setLedDelayed(1, 6);

        #ifdef DEBUG
        Serial.print("@DBGINF: Calib5 @ ");
        Serial.print(v5);
        Serial.print(" mV and read ");
        Serial.print(rd);
        Serial.print(". Max val = ");
        Serial.println(calibMax);
        #endif
    }

    else if(c == CONF_CMD_CALIB_12) {
        // !D<ID><MAX>#
        if(len < 9)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }
        
        auto v12 = str2int(rs::buffer, 4, 8);

        auto rd = analogRead(PIN_M12V);

        auto calibMax = map(1024, 0, rd, 0, v12);

        eec.max_12v = calibMax;
        EEPROM.put(CONF_EEPROM_ADDRESS, eec);

        onboard::setLedDelayed(1, 6);

        #ifdef DEBUG
        Serial.print("@DBGINF: Calib12 @ ");
        Serial.print(v12);
        Serial.print(" mV and read ");
        Serial.print(rd);
        Serial.print(". Max val = ");
        Serial.println(calibMax);
        #endif
    }

    else if(c == CONF_CMD_ID) {
        // !E<ID><NEW_ID><GR_1><GR_2>#
        if(len < 13)
        {
            #ifdef DEBUG
            dbg_nel(c, len);
            #endif

            return;
        }

        auto nid = str2int(rs::buffer, 4, 6);
        auto ng1 = str2int(rs::buffer, 7, 9);
        auto ng2 = str2int(rs::buffer, 10, 12);

        eec.id_single = nid;
        eec.id_group_1 = ng1;
        eec.id_group_2 = ng2;

        EEPROM.put(CONF_EEPROM_ADDRESS, eec);

        eeprom_read();

        onboard::setLedDelayed(1, 6);

        #ifdef DEBUG
        Serial.print("@DBGINF: Changed dev. id to ");
        Serial.print(nid);
        Serial.print("; g1=");
        Serial.print(ng1);
        Serial.print("; g2=");
        Serial.println(ng2);
        #endif
    }
    #endif
}
