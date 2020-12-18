// TASK 3.1

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h" // Where O_WRONLY defined

int tee1(int fd1);

int tee2(int fd1, int fd2);

int
main(int argc, char *argv[]) {

    int fd1, fd2;

    if (argc < 2) {
        printf(2, "Usage: Requires 1 or 2 arguments\n");
        exit(0);
    }

    if (argc == 2) {

        if ((fd1 = open(argv[1], O_WRONLY | O_CREATE)) < 0) {
            printf(1, "tee: cannot open %s\n", argv[1]);
            exit(0);
        }

        // return -1 if write ERROR
        if (tee1(fd1) == -1) {
            printf(2, "tee: write error on %s\n", argv[1]);
        }
    } else {
        if ((fd1 = open(argv[1], O_RDONLY)) < 0) {
            printf(1, "tee: %s doesn't exist\n", argv[1]);
            exit(0);
        }
        if ((fd2 = open(argv[2], O_WRONLY | O_CREATE)) < 0) {
            printf(1, "tee: cannot open %s\n", argv[2]);
            exit(0);
        }

        // return -1 if write ERROR
        if (tee2(fd1, fd2) == -1) {
            printf(2, "tee: write error on %s\n", argv[2]);
        }
        close(fd2);
    }
    close(fd1);
    exit(0);
}

int
tee1(int fd1) {
    int n;
    char buf[512];

    while ((n = read(0, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
        if (write(fd1, buf, n) != n) return -1;
    }
    return 0;
}

int
tee2(int fd1, int fd2) {
    int n;
    char buf[512];

    while ((n = read(fd1, buf, sizeof(buf))) > 0)
        if (write(fd2, buf, n) != n) return -1;

    return 0;
}
