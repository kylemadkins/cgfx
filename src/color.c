#include <stdint.h>
#include <stdlib.h>

#include "color.h"

Color random_color() {
    uint8_t r = rand() & 0xff;
    uint8_t g = rand() & 0xff;
    uint8_t b = rand() & 0xff;
    return (r << 24) | (g << 16) | (b << 8) | 0xff;
}
