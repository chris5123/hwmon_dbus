#include "hwmon_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define SYS_HWMON "/sys/class/hwmon"
#define MAX_PATH 512

static int read_double(const char *path, double *out)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    long long v;
    if (fscanf(f, "%lld", &v) != 1) {
        fclose(f);
        return -1;
    }
    fclose(f);
    *out = (double)v;
    return 0;
}

int hwmon_read_all(sensor_value_t *out, size_t max, size_t *count)
{
    if (!out || max == 0 || !count) return -1;
    *count = 0;

    DIR *d = opendir(SYS_HWMON);
    if (!d) return -1;

    struct dirent *e;
    while ((e = readdir(d))) {
        if (strncmp(e->d_name, "hwmon", 5) != 0)
            continue;

        char hwmon_path[MAX_PATH];
        snprintf(hwmon_path, sizeof(hwmon_path),
                 SYS_HWMON "/%s", e->d_name);

        /* 讀 device name */
        char name_file[MAX_PATH];
        snprintf(name_file, sizeof(name_file),
                 "%s/name", hwmon_path);

        FILE *nf = fopen(name_file, "r");
        if (!nf) continue;

        char dev_name[MAX_NAME] = {0};
        fgets(dev_name, sizeof(dev_name), nf);
        fclose(nf);
        dev_name[strcspn(dev_name, "\n")] = 0;

        DIR *hd = opendir(hwmon_path);
        if (!hd) continue;

        struct dirent *he;
        while ((he = readdir(hd))) {
            const char *fn = he->d_name;
            if (fn[0] == '.') continue;
            if (strstr(fn, "_label")) continue;

            if (!(strncmp(fn, "temp", 4) == 0 ||
                  strncmp(fn, "in", 2) == 0 ||
                  strncmp(fn, "fan", 3) == 0 ||
                  strncmp(fn, "freq", 4) == 0))
                continue;

            char path[MAX_PATH];
            snprintf(path, sizeof(path), "%s/%s", hwmon_path, fn);

            double raw;
            if (read_double(path, &raw) != 0)
                continue;

            const char *unit = "";
            double value = raw;

            if (strncmp(fn, "temp", 4) == 0) {
                value = raw / 1000.0;
                unit = "°C";
            } else if (strncmp(fn, "in", 2) == 0) {
                value = raw / 1000.0;
                unit = "V";
            } else if (strncmp(fn, "fan", 3) == 0) {
                unit = "RPM";
            } else if (strncmp(fn, "freq", 4) == 0) {
                value = raw / 1000000.0;
                unit = "MHz";
            }

            if (*count < max) {
                snprintf(out[*count].name, MAX_NAME, "hwmon%s:%s %s",
                         e->d_name + 5, dev_name, fn);
                out[*count].value = value;
                snprintf(out[*count].unit, MAX_UNIT, "%s", unit);
                (*count)++;
            }
        }
        closedir(hd);
    }

    closedir(d);
    return 0;
}
