#ifndef CONFIG_H
#define CONFIG_H

#include "app_types.h"

#define APP_CONFIG_MAGIC 0x534D5432u

typedef struct {
    uint32_t magic;
    AppConfig value;
    uint32_t checksum;
} StoredConfig;

AppConfig config_defaults(void);
bool config_is_valid(const AppConfig *config);
uint32_t config_checksum(const AppConfig *config);
StoredConfig config_pack(const AppConfig *config);
bool config_unpack(const StoredConfig *stored, AppConfig *config);

#endif
