#include "hwmon_core.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int interval_ms = (argc > 1) ? atoi(argv[1]) : 1000;

    hwmon_snapshot_t snap;

    while (1) {
        printf("\033[H\033[J");

        if (hwmon_scan(&snap) == 0) {
            for (int i = 0; i < snap.device_count; i++) {
                hwmon_device_t *d = &snap.devices[i];
                for (int j = 0; j < d->entry_count; j++) {
                    hwmon_entry_t *e = &d->entries[j];
                    printf("%s:%s %s = %.2f %s\n",
                           d->hwmon_id,
                           d->device,
                           e->name,
                           e->value,
                           e->unit);
                }
            }
        }

        usleep(interval_ms * 1000);
    }
}