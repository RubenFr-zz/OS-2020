#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        printf(2, "Usage: Requires 1 argument\n");
        exit(0);
    }
    int fd;
    if ((fd = open(argv[1], O_CREATE)) < 0) {
        printf(1, "touch: cannot create %s\n", argv[1]);
        exit(0);
    }
    close(fd);
    exit(0);

}
