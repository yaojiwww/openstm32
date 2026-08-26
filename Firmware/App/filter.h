#ifndef FILTER_H
#define FILTER_H

#include "app_types.h"

#define FILTER_MAX_WINDOW 16u

typedef struct {
    uint16_t values[FILTER_MAX_WINDOW];
    uint32_t sum;
    uint8_t window;
    uint8_t count;
    uint8_t index;
} MovingAverage;

void filter_init(MovingAverage *filter, uint8_t window);
uint16_t filter_push(MovingAverage *filter, uint16_t value);

#endif
