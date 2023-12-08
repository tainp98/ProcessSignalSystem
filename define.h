#ifndef DEFINE_H
#define DEFINE_H
#include <iostream>
enum class SensorType : unsigned char{
    SENSOR1 = 0,
    SENSOR2
};

template<typename T>
union Type2Byte{
    T value;
    char data[sizeof (T)];
};

static bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

#endif // DEFINE_H
