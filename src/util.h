#pragma once

unsigned int str2int(const char* str, unsigned int from, unsigned int to)
{
    unsigned int ret = 0;

    for(; from <= to; ++from)
    {
        ret = ret * 10 + (str[from] - '0');
    }
    return ret;
}