#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const long LINES = 429496729;

int main(int argc, char *argv[]) {
    long fileOffsets[300];
    int fd = 0, i = 1, j = 0, lineNumber = 0, lineLength[300] = {-1};
    char c = 0, *buf;
    if((buf = (char*) malloc(257 * sizeof(char))) == NULL) {
        exit(1);
    }
    if (argc < 2) {
        perror("Usage: filename as argument");
        exit(2);
    }
    if((fd = open(argv[1], O_RDONLY)) == -1) {
        perror(strcat("Input file doesn't exist - ", argv[1]));
        exit(1);
    }
    fileOffsets[0] = 0L;
    fileOffsets[1] = 0L;
       while(read(fd, &c, 1)) {
        if(i == LINES - 1) {
            perror("File is large");
        }
        if(c == '\n') {
            j++;
            lineLength[i++] = j;
            fileOffsets[i] = lseek(fd, 0L, SEEK_CUR);
            j = 0;
        }
        else {
            j++;
        }
    }
    int bufSize = 257;
    while(printf("Enter line number : ") && scanf("%d", &lineNumber)) {
        if(lineNumber == 0) {
            exit(0);
        }
        if(lineNumber < 0 || lineNumber > LINES || (lineLength[lineNumber] == -1)) {
            fprintf(stderr, "Wrong line number!!!\n");
            continue;
        }
        lseek(fd, fileOffsets[lineNumber], SEEK_SET);
        if (lineLength[lineNumber] > bufSize) {
            if(realloc(buf, lineLength[lineNumber] * sizeof(char))) {
                exit(1);
            }
            bufSize = lineLength[lineNumber];
        }
        if(read(fd, buf, lineLength[lineNumber])) {
            write(1, buf, lineLength[lineNumber]);
        }
        else {
            fprintf(stderr, "Wrong line number!!!\n");
        }
    }
    free(buf);
    return 0;
}
