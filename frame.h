#pragma once

#include "CONF.h"
// #include "timer.h"
#include "mapping.h"

namespace frame
{
    unsigned char frame[3 * CONF_FRAME_SIZE];

    int current = 0;

    inline void set_pwm(int f_idx)
    {
        auto addr = f_idx * 3;
        
        // timer::set(
        //     frame[addr],
        //     frame[addr + 1],
        //     frame[addr + 2]
        // );
        mapping::set_mapped(
            frame[addr],
            frame[addr + 1],
            frame[addr + 2]
        );
        current = f_idx;
    }

    inline void set_frame(
        int f_idx,
        const unsigned char o1,
        const unsigned char o2,
        const unsigned char o3
        )
    {
        auto addr = f_idx * 3;

        frame[addr] = o1;
        frame[addr + 1] = o2;
        frame[addr + 2] = o3;
    }
}