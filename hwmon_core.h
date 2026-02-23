#ifndef HWMON_CORE_H
#define HWMON_CORE_H

#include <stddef.h>

#define MAX_NAME 64
#define MAX_UNIT 8

typedef struct {
    char name[MAX_NAME];
    double value;
    char unit[MAX_UNIT];
} sensor_value_t;

/**
 * Read all hwmon sensors
 * @param out array of sensor_value_t
 * @param max maximum number of sensors
 * @param count returns the actual number of sensors read
 * @return 0 on success, <0 on failure
 */
int hwmon_read_all(sensor_value_t *out, size_t max, size_t *count);

#endif