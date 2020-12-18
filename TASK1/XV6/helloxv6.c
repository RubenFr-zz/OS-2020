#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[]) {
    if (argc < 1) {
        printf(2, "Usage: mkdir files...\n");
        exit(0);
    }

    printf(1, "Hello, XV6!\n");
//    exit(0);
}
