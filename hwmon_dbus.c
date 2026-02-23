#include "hwmon_core.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define MAX_SENSOR 64
#define INTERVAL_SEC 1
#define LOG_FILE "hwmon.log"

static void print_timestamp(FILE *out)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    time_t sec = ts.tv_sec;
    struct tm tm;
    gmtime_r(&sec, &tm);

    fprintf(out, "[UTC: %04d-%02d-%02d %02d:%02d:%02d]\n",
           tm.tm_year + 1900,
           tm.tm_mon + 1,
           tm.tm_mday,
           tm.tm_hour,
           tm.tm_min,
           tm.tm_sec);
}

int main(void)
{
    while (1) {
        // ===== 終端固定刷新區塊 =====
        printf("\033[H");   // 游標回左上
        printf("\033[J");   // 清掉螢幕內容

        // 打開 log 檔案 (追加模式)
        FILE *logf = fopen(LOG_FILE, "a");
        if (!logf) {
            perror("fopen log");
            return 1;
        }

        // timestamp 同時印螢幕 + log
        print_timestamp(stdout);
        print_timestamp(logf);

        sensor_value_t sensors[MAX_SENSOR];
        size_t count = 0;
        if (hwmon_read_all(sensors, MAX_SENSOR, &count) == 0) {
            for (size_t i = 0; i < count; i++) {
                printf("%s = %.2f %s\n",
                       sensors[i].name,
                       sensors[i].value,
                       sensors[i].unit);

                fprintf(logf, "%s = %.2f %s\n",
                        sensors[i].name,
                        sensors[i].value,
                        sensors[i].unit);
            }
        }

        fflush(stdout);
        fflush(logf);
        fclose(logf);

        sleep(INTERVAL_SEC);
    }

    return 0;
}