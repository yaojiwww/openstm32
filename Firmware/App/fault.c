#include "fault.h"

void fault_init(FaultManager *manager) {
    manager->active = FAULT_NONE;
    manager->occurrence_count = 0;
}

void fault_raise(FaultManager *manager, FaultCode code) {
    if ((manager->active & (uint32_t)code) == 0u) ++manager->occurrence_count;
    manager->active |= (uint32_t)code;
}

void fault_clear(FaultManager *manager, FaultCode code) {
    manager->active &= ~(uint32_t)code;
}

bool fault_is_active(const FaultManager *manager, FaultCode code) {
    return (manager->active & (uint32_t)code) != 0u;
}
