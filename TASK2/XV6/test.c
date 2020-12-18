#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int pid;
    int k, n;
    double x, z;

    if (argc != 2) {
        printf(2, "usage: %s n\n", argv[0]);
    }

    n = atoi(argv[1]);

    for ( k = 0; k < n; ++k) {
        if ((pid = fork ()) < 0 ) {
            printf(2, "%d failed in fork!\n", getpid());
            exit();
        } else if (pid == 0) {
            // child
            setpriority(50-10*k);
            sleep(4);
            for ( z = 0; z < 5000000; z+=0.01){
                x =  x + 3.14 * 89.64;   // useless calculations to consume CPU time
            }
            printf(1, "process %d finished - priority: %d\n", getpid(), 50-10*k);
            exit();
        }
    }

    for (k = 0; k < n; k++) {
        wait();
    }

    exit();
}
/*
int
main(int argc, char *argv[])
{
    int father_new_priority = (argc < 3) ? 51 : atoi(argv[1]);
    int child_new_priority = (argc < 3) ? 20 : atoi(argv[2]);

    int father_old_priority = setpriority(father_new_priority);
    printf(1, "I'm the father (%d)- Changed priority %d -> %d\n", getpid(), father_old_priority, father_new_priority);

    int pid;
    if((pid = fork()) < 0){
        printf(1, "Failed!\n");
        exit();
    }
    else if(pid == 0){
        int child_old_priority = setpriority(child_new_priority);
        printf(1, "I'm the child (%d)- Changed priority %d -> %d\n", getpid(), child_old_priority, child_new_priority);

        for(int j=0; j<1000000; j++)
            for(int j=0; j<1000000; j++){}
        printf(1, "Child finished running (pid %d)\n", getpid());
        exit();
    }
    else {
        for(int j=0; j<1000000; j++)
            for(int j=0; j<1000000; j++){}
        printf(1, "Parent finished running (pid %d)\n", getpid());
    }
    wait();
    exit();
}
*/
//
// Created by eranaf on 14/12/2020.
//
//
//#include "types.h"
//#include "stat.h"
//#include "user.h"
//
//#include "fcntl.h"
//
////passing command line arguments
//
//int main(int argc, char **argv)
//{
//    int newprior = atoi(argv[1]);
//    int myPid = getpid();
//    int oldPrior = setpriority(newprior);
//    printf(1, "I'm the father - pid %d! change my priority to %d from %d\n",myPid,newprior,oldPrior);
//
//    int pid = fork();
//    for(int j=0; j<1000000; j++){}
//
//
//    if(pid == 0){
//        int newprior = atoi(argv[2]);
//        int myPid = getpid();
//        int oldPrior = setpriority(newprior);
//        printf(1, "I'm the child - pid %d! change my priority to %d from %d\n",myPid,newprior,oldPrior);
//        for(int i=0;i<10;i++){
//            for(int j=0; j<1000000; j++){}
//            printf(1, "%d - <pid = %d, prior= %d>\n ", i, myPid,newprior);
//        }
//        exit();
//    } else if(pid < 0){
//        printf (1, "This is fork failed\n");
//        exit();
//    }else{
//        for(int i=0;i<10;i++){
//            for(int j=0; j<1000000; j++){}
//            printf(1, "%d - <pid = %d, prior= %d>\n ", i, myPid,newprior);
//        }
//    }
//    wait();
//    exit();
//}