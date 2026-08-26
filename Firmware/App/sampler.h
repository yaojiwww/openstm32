#ifndef SAMPLER_H
#define SAMPLER_H

#include "filter.h"

typedef struct {
    MovingAverage filter;
    uint16_t vref_mv;
    uint16_t adc_full_scale;
} Sampler;

void sampler_init(Sampler *sampler, uint8_t filter_window, uint16_t vref_mv,
                  uint16_t adc_full_scale);
SampleRecord sampler_process(Sampler *sampler, uint16_t raw, uint32_t now_ms);

#endif
