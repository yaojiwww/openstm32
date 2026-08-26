#ifndef FAULT_H
#define FAULT_H

#include "app_types.h"

typedef enum {
    FAULT_NONE = 0,
    FAULT_SD_MISSING = 1u << 0,
    FAULT_SD_FULL = 1u << 1,
    FAULT_UART_TIMEOUT = 1u << 2,
    FAULT_CONFIG_CORRUPT = 1u << 3,
    FAULT_ADC_TIMEOUT = 1u << 4
} FaultCode;

typedef struct {
    uint32_t active;
    uint32_t occurrence_count;
} FaultManager;

void fault_init(FaultManager *manager);
void fault_raise(FaultManager *manager, FaultCode code);
void fault_clear(FaultManager *manager, FaultCode code);
bool fault_is_active(const FaultManager *manager, FaultCode code);

#endif
