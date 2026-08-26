#include "sampler.h"

void sampler_init(Sampler *sampler, uint8_t filter_window, uint16_t vref_mv,
                  uint16_t adc_full_scale) {
    filter_init(&sampler->filter, filter_window);
    sampler->vref_mv = vref_mv;
    sampler->adc_full_scale = adc_full_scale;
}

SampleRecord sampler_process(Sampler *sampler, uint16_t raw, uint32_t now_ms) {
    SampleRecord record;
    record.timestamp_ms = now_ms;
    record.raw = raw;
    record.filtered = filter_push(&sampler->filter, raw);
    record.millivolts = (uint16_t)(((uint32_t)record.filtered * sampler->vref_mv) /
                                  sampler->adc_full_scale);
    return record;
}
