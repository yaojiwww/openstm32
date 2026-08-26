#include "filter.h"

void filter_init(MovingAverage *filter, uint8_t window) {
    filter->sum = 0;
    filter->count = 0;
    filter->index = 0;
    filter->window = (window == 0) ? 1 : window;
    if (filter->window > FILTER_MAX_WINDOW) {
        filter->window = FILTER_MAX_WINDOW;
    }
    for (uint8_t i = 0; i < FILTER_MAX_WINDOW; ++i) {
        filter->values[i] = 0;
    }
}

uint16_t filter_push(MovingAverage *filter, uint16_t value) {
    if (filter->count < filter->window) {
        ++filter->count;
    } else {
        filter->sum -= filter->values[filter->index];
    }
    filter->values[filter->index] = value;
    filter->sum += value;
    filter->index = (uint8_t)((filter->index + 1u) % filter->window);
    return (uint16_t)(filter->sum / filter->count);
}
