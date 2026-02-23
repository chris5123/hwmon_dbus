#ifndef HWMON_TYPES_H
#define HWMON_TYPES_H

#define HWMON_MAX_DEVICES   16
#define HWMON_MAX_ENTRIES   64
#define HWMON_NAME_LEN     64
#define HWMON_LABEL_LEN    32

typedef enum {
    HWMON_TEMP,
    HWMON_VOLT,
    HWMON_FAN,
    HWMON_FREQ,
    HWMON_UNKNOWN
} hwmon_type_t;

typedef struct {
    char        name[HWMON_LABEL_LEN];  // temp1_input
    hwmon_type_t type;
    double      value;
    char        unit[8];                // °C / V / RPM / MHz
} hwmon_entry_t;

typedef struct {
    char            hwmon_id[16];       // hwmon6
    char            device[HWMON_NAME_LEN]; // amdgpu
    int             entry_count;
    hwmon_entry_t   entries[HWMON_MAX_ENTRIES];
} hwmon_device_t;

typedef struct {
    int              device_count;
    hwmon_device_t   devices[HWMON_MAX_DEVICES];
} hwmon_snapshot_t;

#endif