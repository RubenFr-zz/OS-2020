#include "types.h"
#include "stat.h"
#include "user.h"

int
main() {
//    printf(1, "\033[H\033[J"); // Clear everything visible on the screen
    printf(1, "\033[2J\033[1;1H"); // Print empty lines until nothing on the screen
    exit(0);
}
