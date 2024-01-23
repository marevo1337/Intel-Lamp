#include "BitOperation.h"

BOOL is_bits_even(char number)
{
    int count = 0;
    while (number) {
        count += number & 1;
        number >>= 1;
    }

    return count % 2;
}

BOOL is_auxiliary_carry_set(int result) 
{
    return (result & 0x10) != 0;
}
