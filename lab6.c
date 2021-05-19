#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define bufSize 257
#define charSize 100

void closeFile(int fileDescriptor) {
    if (close(fileDescriptor) == -1) {
        if (errno == EINTR) {
            if (close(fileDescriptor) == -1 && errno != EINTR) {
                printf("Error close file");
            }
        } else {
            printf("Error close file");
        }
    }
}

int readFile(const int fd, char *buf, int sizeBuf) {
    int count;
    if ((count = read(fd, buf, sizeBuf)) == -1) {
        if (errno == EINTR) {
            // Системный вызов был прерван сигналом до того, как был записан хотя бы один байт данных.
            if ((count = read(fd, buf, sizeBuf)) == -1 && errno != EINTR) {
                printf("Error read a file");
                return -1;
            }
        } else {
            printf("Error read a file");
            return -1;
        }
    }
    return count;
}

int writeCons(char buf[], int sizeBuf){
    if (write(1, buf, sizeBuf) == -1) {
        if (errno == EINTR) {
            // Системный вызов был прерван сигналом до того, как был записан хотя бы один байт данных.
            if(write(1, buf, sizeBuf) == -1 && errno != EINTR) {
                printf("Error write");
                return 1;
            }
        } else{
            printf("Error write");
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int fileDescriptor1, fileDescriptor2;
    long displ[500] = {0};
    int countAll = 1;
    int i = 1, j = 0, lineNumber, lineLen[500] = {0};
    char c[bufSize], buf[bufSize];
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    if ((fileDescriptor1 = open("/dev/tty", O_RDONLY | O_NDELAY)) == -1) {
        printf("Error, can't open file with name - '/dev/tty'\n");
        return 1;
    }
    fd_set rfd;
    FD_SET(fileDescriptor1, &rfd);

    if ((fileDescriptor2 = open(argv[1], O_RDONLY)) == -1) {
        if (argv[1] == NULL) {
            printf("Write a name of file\n");
        } else {
            printf("No find file with name - '%s'\n", argv[1]);
        }
        closeFile(fileDescriptor1);
        return 1;
    }

    int maxFileDescriptor;
    if (fileDescriptor1 > fileDescriptor2) {
        maxFileDescriptor = fileDescriptor1;
    } else {
        maxFileDescriptor = fileDescriptor2;
    }

    int count = readFile(fileDescriptor2, c, charSize);
    while (count) {
        for (int k = 0; k < count; k++) {
            if (c[k] == '\n') {
                j++;
                lineLen[i++] = j;
                displ[i] = countAll;
                j = 0;
            } else if (c[k] != '\0') {
                j++;
            }
            if (c[k] != '\0') {
                countAll++;
            }
        }
        if (count == -1) {
            closeFile(fileDescriptor1);
            closeFile(fileDescriptor2);
            return 0;
        }
        count = readFile(fileDescriptor2, c, charSize);
    }

    while (1) {
        printf("You have 5 seconds to enter a line number\n");
        if (!select(maxFileDescriptor + 1, &rfd, NULL, NULL, &tv)) {
            if(lseek(fileDescriptor2, SEEK_SET, 0) == -1) {
                printf("lseek error\n");
                break;
            }
            while ((i = readFile(fileDescriptor2,buf,bufSize)) > 0) {
                if (writeCons(buf, i)) {
                    break;
                }
            }
            break;
        } else {
            i = readFile(fileDescriptor1, buf, bufSize);
            if (i == -1) {
                break;
            }
            buf[i] = '\0';
            lineNumber = atoi(buf);
            if (lineNumber <= 0) {
                printf("Bad line number\n");
                break;
            }
            if (lseek(fileDescriptor2, displ[lineNumber], 0) == -1) {
                printf("lseek error\n");
                break;
            }
            count = readFile(fileDescriptor2, buf, lineLen[lineNumber]);
            if (count) {
                if (writeCons(buf, lineLen[lineNumber])) {
                    break;
                }
            }
            else {
                printf("Bad Line Number\n");
            }
        }
    }
    closeFile(fileDescriptor1);
    closeFile(fileDescriptor2);
    return 0;
}
