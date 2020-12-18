#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    int pid = fork();
    if(pid == 0){
        printf(1, "I'm the child!\n");
        exit(20);
    }
    else if(pid < 0){
        printf(1, "Failed!\n");
        exit(0);
    }
    int status;
    wait(&status);
    printf(1, "I'm the parent, and got child status: %d\n", status);
}
