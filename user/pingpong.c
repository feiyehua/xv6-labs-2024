/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 01:04:51
 * @LastEditTime : 2025-08-22 01:18:25
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : pingpong.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int p1[2];
    pipe(p1); // Create Pipe between the processors
    int p2[2];
    pipe(p2);

    int pid = fork();
    if (pid == 0) // The child process
    {
        pid = getpid();
        // Receive Ping
        char a;
        read(p1[0], &a, 1); // p[0] is the read descriptor
        close(p1[0]);
        close(p1[1]);
        fprintf(1, "%d: received ping\n", pid);

        // Send Pong
        write(p2[1], "v", 1);
        close(p2[0]);
        close(p2[1]);
    }
    else
    {
        pid = getpid();
        // Send Ping
        write(p1[1], "x", 1);
        close(p1[0]);
        close(p1[1]);

        // Receive Pong
        char a;
        read(p2[0], &a, 1);
        fprintf(1, "%d: received pong\n", pid);
        close(p2[0]);
        close(p2[1]);
    }
}