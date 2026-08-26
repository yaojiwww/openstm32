#ifndef STORAGE_H
#define STORAGE_H

#include "app_types.h"

typedef bool (*StorageLineSink)(const char *line, void *context);

bool storage_mount(void);
void storage_unmount(void);
bool storage_append(const SampleRecord *record);
bool storage_replay(StorageLineSink sink, void *context);

#endif
