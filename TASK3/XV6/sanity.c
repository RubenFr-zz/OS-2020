#include "types.h"
#include "user.h"
#include "fcntl.h"



int fibonacci(int num){
    if (num == 0 || num == 1){
        return 1;
    }
    else{
        return fibonacci(num-1) + fibonacci(num-2);
        
    }
}


int Busy(void){
    return fibonacci(30);
}


void simple_handler1(int pid, int value)
{
    printf(1, "handler1 exec, (pid == %d), (value == %d)\n", pid, value);
}

void simple_handler2(int pid, int value)
{

    int ans2 = small_task_calc_only();
    printf(2, "handler2 exec, (pid, value) == (%d, %d), (fibonacci(30) == %d)\n", pid, value, ans2);
}


int main(void){
    
    int child_pid;
    
    //test 1 - check that signal system call works with SIG_IGN - process ignores OTHER_SIG
    printf(1, "1: test\n");
    child_pid = fork();
    if(!child_pid) // child pid
    {
        sigset(&simple_handler1);
        fibonacci(30);
        printf(1, "1: child exiting\n"); //the process ignored the signal
        exit();
    }
    else // parent
    {
        fibonacci(27);
        sigsend(child_pid, 1);
        wait();
    }
    printf(1, "1: test 1 passed\n"); //the process ignored the signal

    //test 2 - check that user signal handler works
    printf(1, "2: test\n");
    child_pid = fork();
    if(!child_pid)  // child
    {
        sigset(&simple_handler2);
        fibonacci(30);
        printf(1, "2: child exiting\n"); //the process ignored the signal
        exit();
    }
    else{
        fibonacci(27);
        sigsend(child_pid, 7);
        wait();
    }
    printf(1, "2: test 2 passed\n"); //the process ignored the signal

    //test 2 - check that user signal handler works
    printf(1, "3: test\n");
    child_pid = fork();
    if(!child_pid)  // child
    {
        sigset(&simple_handler2);
        fibonacci(30);
        printf(1, "3: child exiting\n"); //the process ignored the signal
        exit();
    }
    else{
        fibonacci(27);
        sigsend(child_pid, 1);
        sigsend(child_pid, 2);
        sigsend(child_pid, 3);
        wait();
    }
    printf(1, "3: test 3 passed\n"); //the process ignored the signal


    exit();
} 
















