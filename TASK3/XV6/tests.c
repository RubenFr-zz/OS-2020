#include "types.h"
#include "user.h"
#include "fcntl.h"


int fibonacci(int num){
    if (num == 0 || num == 1) return 1;
    else return fibonacci(num-1) + fibonacci(num-2);
}

int Busy(int lvl){
    return fibonacci(lvl);
}

void
simple_handler1(int pid, int value){
    printf(1, "handler1 exec, sender = %d, value = %d\n", pid, value);
}

void
simple_handler2(int pid, int value){
    printf(2, "handler2 exec, sender = %d, value = %d, (fibonacci(value) == %d)\n", pid, value, fibonacci(value));
}

int main(void){

    int child_pid;
    int father_pid = getpid();
    sig_handler handler;

    // Test 1: Check that the element returned by sigset is indeed the previous signal handler
    printf(1, "1: Test 1 Start\n");
    handler = sigset(&simple_handler1);
    printf(1, "1: Signal handler changed from %d to %d\n", handler, &simple_handler1);
    handler = sigset(&simple_handler2);
    printf(1, "1: Signal handler changed from %d to %d\n", handler, &simple_handler2);
    handler(1, 2);
    handler = sigset((sig_handler) -1);
    printf(1, "1: Signal handler changed from %d to %d\n", handler, -1);
    handler(1, 2);
    printf(1, "Test 1 passed!\n\n");


    // Test 2: Check that when exiting, the child will handle the signal sent by his father with the value 100
    printf(1, "2: Test 2 Start\n");
    child_pid = fork();
    if(child_pid == 0) // child pid
    {
        handler = sigset(&simple_handler1);
        printf(1, "2: I'm Child, Signal handler changed from %d to %d\n", handler, &simple_handler1);
        Busy(30);  // Busy
        printf(1, "2: Child exiting\n"); //the process ignored the signal
        exit();
    }
    else // parent
    {
        Busy(30); // Busy
        printf(1, "Father (pid %d) is sending a signal to its Child (pid %d)...\n", father_pid, child_pid);
        sigsend(child_pid, 1000);
        wait();
        printf(1, "2: Father exiting\n");
    }
    printf(1, "Test 2 passed!\n\n");


    //test 3 - Check that pending signals' stack works (father sends 3 signals)
    printf(1, "3: Test 3 Start\n");
    child_pid = fork();
    if(child_pid == 0)  // child
    {
        handler = sigset(&simple_handler2);
        printf(1, "3: I'm Child, Signal handler changed from %d to %d\n", handler, (int) &simple_handler2);
        Busy(35);
        printf(1, "3: Child exiting\n"); //the process ignored the signal
        exit();
    }
    else{
        Busy(30);
        printf(1, "Father (pid %d) is sending a signal to its Child (pid %d)...\n", father_pid, child_pid);
        sigsend(child_pid, 20);
        sigsend(child_pid, 21);
        sigsend(child_pid, 22);
        wait();
        printf(1, "3: Father exiting\n");
    }
    printf(1, "Test 3 passed!\n\n"); //the process ignored the signal
    
    // Test 4 - Signal lost, the son exit before the father send the signal
    printf(1, "4: Test 4 Start\n");
    child_pid = fork();
    if(child_pid == 0) // child pid
    {
        handler = sigset(&simple_handler1);
        printf(1, "4: I'm Child, Signal handler changed from %d to %d\n", handler, &simple_handler1);
        printf(1, "4: Child exiting\n"); //the process ignored the signal
        exit();
    }
    else // parent
    {
        Busy(30); // Busy
        printf(1, "4: Father (pid %d) is sending a signal to its Child (pid %d)...\n", father_pid, child_pid);
        int sent = sigsend(child_pid, 1000);
        printf(1, "4: sent = %d -> %s\n", sent, sent ? "Signal sent!" : "Sending failed");
        wait();
        printf(1, "4: Father exiting\n");
    }
    printf(1, "Test 4 passed!\n\n");

    exit();
}
















