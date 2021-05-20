#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define bufSize 100

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
    int fileDescriptor;
    long displ[500] = {0};
    int countAll = 1;
    int i = 1, j = 0, lineNumber, lineLen[500] = {0};
    char c[bufSize], buf[257];
    if ((fileDescriptor = open(argv[1], O_RDONLY)) == -1) {
        if (argv[1] == NULL) {
            printf("Write a name of file\n");
        } else {
            printf("No find file with name - '%s'\n", argv[1]);
        }
        return 1;
    }
    int count;
    count = readFile(fileDescriptor, c, bufSize);
    while (count > 0) {
        for(int k = 0; k < count; k++) {
            if (c[k] == '\n') {
                j++;
                lineLen[i++] = j;
                displ[i] = countAll;
                j = 0;
            }
            else if (c[k] != '\0') {
                j++;
            }
            if (c[k] != '\0') {
                countAll++;
            }
        }
        count = readFile(fileDescriptor, c, bufSize);
        if (count == -1) {
            exit(1);    
        }
    }
    while (printf("Line number : ") && scanf("%d", &lineNumber)) {
        if (lineNumber <= 0) {
            printf("Bad line number\n");
            break;
        }
        if (lseek(fileDescriptor, displ[lineNumber], 0) == -1) {
            printf("lseek error\n");
            break;
        }
        count = readFile(fileDescriptor, buf, lineLen[lineNumber]);
        if (count) {
            if (writeCons(buf, lineLen[lineNumber])) {
                break;
            }
        }
        else {
            printf("Bad Line Number\n");
        }
    }
    if (close(fileDescriptor) == -1) {
        if (errno == EINTR){
            if (close(fileDescriptor) == -1 && errno != EINTR){;
                printf("Error close file");
            }
        } else {
            printf("Error close file");
        }
    }
}
