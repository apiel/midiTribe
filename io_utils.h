#ifndef IO_UTILS_H_
#define IO_UTILS_H_

#include <Arduino.h>

bool intArrayStartWith(const uint8_t *start, const uint8_t *data, byte ignore)
{
    for (uint16_t i = 0; i < 10 && start[i] != '\0'; i++)
    {
        if (start[i] == ignore && data[i] != '\0' && data[i] != start[i])
        {
            return false;
        }
    }
    return true;
}

#endif