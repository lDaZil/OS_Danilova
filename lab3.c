#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    FILE *file;
    printf("Real user id: %d,  effective user id: %d\n", getuid(), geteuid());
    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Can't open file");
        return 1;
    } else {
        printf("File opens sucessfully\n");
    }
    fclose(file);

    seteuid(getuid());

    printf("New real user id: %d, new effective user id: %d\n", getuid(), geteuid());

    file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Can't open file (2)");
        return 1;
    } else {
       printf("File opens sucessfully\n");
    }
    fclose(file);
    return 0;
}
