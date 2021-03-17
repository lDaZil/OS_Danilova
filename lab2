#include <stdlib.h>
#include <time.h>
#include <stdio.h>
extern char *tzname[];

int main() {
    if (putenv("TZ=America/Los_Angeles") != 0) {
        printf("Putenv error!");
        return 1;
    }
    time_t now;
    if (time(&now) == -1) {
        printf("Convert time error!");
        return 1;
    }
    struct tm *sp;
    sp = localtime(&now);
    printf("%02d/%02d/%d %02d:%02d:%02d %s\n",
           sp->tm_mday,
           sp->tm_mon + 1,
           sp->tm_year + 1900,
           sp->tm_hour,
           sp->tm_min,
           sp->tm_sec,
           tzname[sp->tm_isdst]);
    return 0;
}
