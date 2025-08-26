/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-26 14:47:37
 * @LastEditTime : 2025-08-27 00:50:37
 * @LastEditors  : FeiYehua
 * @Description  : 
 * @FilePath     : sigalarm.c
 *      © 2024 FeiYehua
 */

#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "types.h"
#include "spinlock.h"
#include "param.h"
#include "proc.h"

uint64 sys_sigalarm()
{
    // printf("sigalrm called!\n");
    int interval;
    sighandler_t handler;
    // Get the corresponding arguments
    argint(0, &interval);
    argaddr(1, (uint64 *)&handler);
    struct proc *p = myproc();
    p->interval = interval;
    p->handler = handler;
    return 0;
}

uint64 sys_sigreturn()
{
    printf("sigreturn called\n");
    // We should use this function to restore the original trapframe and restore the PC
    struct proc *p = myproc();
    // Restore the program counter in user space to the state prior to calling the signal handler
    p->trapframe->epc = p->trapframe->old_epc;

    // Restore %ra and %s0
    copyin(p->pagetable, (char *)&p->trapframe->ra, p->trapframe->sp + 0x8, 8);
    copyin(p->pagetable, (char *)&p->trapframe->s0, p->trapframe->sp, 8);

    // Restore stack pointer to the state prior to calling the signal handler
    p->trapframe->sp += 0x10;
    return 0;
}