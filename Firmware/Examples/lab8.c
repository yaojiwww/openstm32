#include "board_port.h"

#define WAVE_POINTS 32u

/* One period of a 12-bit sine-like waveform; TIM6 controls output frequency. */
static const uint16_t test_wave[WAVE_POINTS] = {
    2048, 2447, 2831, 3185, 3495, 3748, 3936, 4052,
    4095, 4052, 3936, 3748, 3495, 3185, 2831, 2447,
    2048, 1648, 1264, 910,  600,  347,  159,  43,
    0,    43,   159,  347,  600,  910,  1264, 1648,
};

bool lab8_start_test_wave(void) {
    /* Start TIM6 TRGO before this call in CubeMX-generated code. */
    return board_dac_dma_start(test_wave, WAVE_POINTS);
}

/* ADC DMA completion should only publish a buffer-ready flag. Processing and
 * SD writes remain in the main loop, never inside the interrupt callback. */
