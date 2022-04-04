#ifndef IO_UTILS_H_
#define IO_UTILS_H_

#include <Arduino.h>

#define between(x, a, b) (((a) <= (x)) && ((x) <= (b)))

byte mod(int8_t a, byte b)
{
    int8_t c = a % b;
    return (c < 0) ? c + b : c;
}

// bool intArrayStartWith(const uint8_t *start, const uint8_t *data, byte ignore)
// {
//     for (uint16_t i = 0; i < 10 && start[i] != '\0'; i++)
//     {
//         if (start[i] != ignore && data[i] != '\0' && data[i] != start[i])
//         {
//             return false;
//         }
//     }
//     return true;
// }

void printIntArray(const uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        Serial.printf("%d,", data[i]);
    }
}

// void printIntArray(const uint8_t *data)
// {
//     for (uint16_t i = 0; i < 4 && data[i] != '\0'; i++)
//     {
//         Serial.printf("%d,", data[i]);
//     }
// }

#endif