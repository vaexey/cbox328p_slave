#pragma once

#include "CONF.h"
#include "timer.h"

namespace mapping
{
    // idx:
    // i + 0 -> R
    // i + 256 -> G
    // i + 512 -> B
    unsigned char map[3 * CONF_MAPPING_SIZE];

    inline void set_mapped(const unsigned char o1, const unsigned  char o2, const unsigned  char o3)
    {
        timer::set(
            map[o1],
            map[o2 + CONF_MAPPING_SIZE],
            map[o3 + 2 * CONF_MAPPING_SIZE]
        );
    }

    void build()
    {
        for(int i = 0; i < CONF_MAPPING_SIZE; i++)
        {
            map[i] = i;
            map[i + CONF_MAPPING_SIZE] = i;
            map[i + 2 * CONF_MAPPING_SIZE] = i;
        }
    }

    inline void set_map_value(const int map_idx, const unsigned char value)
    {
        map[map_idx] = value;
    }
}