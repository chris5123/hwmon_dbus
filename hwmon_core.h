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
 * 讀取所有 hwmon sensor
 * @param out array of sensor_value_t
 * @param max 最大數量
 * @param count 回傳實際讀到的 sensor 數量
 * @return 0 成功, <0 失敗
 */
int hwmon_read_all(sensor_value_t *out, size_t max, size_t *count);

#endif