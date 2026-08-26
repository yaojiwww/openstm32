#include "config.h"

AppConfig config_defaults(void) {
    AppConfig value = {
        .sample_rate_hz = 100,
        .alarm_threshold_mv = 2500,
        .filter_window = 8,
    };
    return value;
}

bool config_is_valid(const AppConfig *config) {
    return config != NULL && config->sample_rate_hz >= 1 &&
           config->sample_rate_hz <= 10000 &&
           config->alarm_threshold_mv <= 3300 &&
           config->filter_window >= 1 && config->filter_window <= 16;
}

uint32_t config_checksum(const AppConfig *config) {
    uint8_t bytes[7] = {
        (uint8_t)config->sample_rate_hz,
        (uint8_t)(config->sample_rate_hz >> 8u),
        (uint8_t)(config->sample_rate_hz >> 16u),
        (uint8_t)(config->sample_rate_hz >> 24u),
        (uint8_t)config->alarm_threshold_mv,
        (uint8_t)(config->alarm_threshold_mv >> 8u),
        config->filter_window,
    };
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < sizeof(bytes); ++i) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

StoredConfig config_pack(const AppConfig *config) {
    StoredConfig stored = {.magic = APP_CONFIG_MAGIC, .value = *config};
    stored.checksum = config_checksum(config);
    return stored;
}

bool config_unpack(const StoredConfig *stored, AppConfig *config) {
    if (stored == NULL || config == NULL || stored->magic != APP_CONFIG_MAGIC ||
        stored->checksum != config_checksum(&stored->value) ||
        !config_is_valid(&stored->value)) {
        return false;
    }
    *config = stored->value;
    return true;
}
