/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-26 14:47:37
 * @LastEditTime : 2025-08-26 15:54:27
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
    
    return 0;
}