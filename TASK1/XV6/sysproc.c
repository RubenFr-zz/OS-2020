#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void) {
    return fork();
}

//int
//sys_exit(void) {
//    exit();
//    return 0;  // not reached
//}

// TASK 4.1
int
sys_exit(void) {
    int zero;
    if (argint(0, &zero) < 0)
        return -1;
    exit(zero);
    return 0;  // not reached
}

// TASK 4.2
int
sys_wait(void) {
    int *status;
    if (argptr(0, (char **) &status, 4) < 0)  // 4 - Size of int (4 bytes)
        return -1;

    return wait(status);
}

int
sys_kill(void) {
    int pid;

    if (argint(0, &pid) < 0)
        return -1;
    return kill(pid);
}

int
sys_getpid(void) {
    return myproc()->pid;
}

int
sys_sbrk(void) {
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    addr = myproc()->sz;
    if (growproc(n) < 0)
        return -1;
    return addr;
}

int
sys_sleep(void) {
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
        return -1;
    acquire(&tickslock);
    ticks0 = ticks;
    while (ticks - ticks0 < n) {
        if (myproc()->killed) {
            release(&tickslock);
            return -1;
        }
        sleep(&ticks, &tickslock);
    }
    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void) {
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);
    return xticks;
}

int
sys_getinfo(void) {
    print_ptable();
    return 0;
}

