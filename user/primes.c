/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 10:25:08
 * @LastEditTime : 2025-08-22 12:04:32
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : primes.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int p[100][2]; // Pipe array
void primes(int) __attribute__((noreturn));

// Use p[n][0] as pipe read descriptor, p[n+1][1] as pipe write descriptor
void primes(int n)
{
    close(p[n][1]);
    int prime, t;
    if (read(p[n][0], &prime, sizeof(int)) < sizeof(int)) // Get the first number (the prime)
    {
        close(p[n][0]);
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);
    pipe(p[n + 1]);
    int pid = fork();
    if (pid == 0)
    {
        close(p[n][0]);
        primes(n + 1);
    }
    else
    {
        close(p[n + 1][0]);
        while (read(p[n][0], &t, sizeof(int)) == sizeof(int))
        {
            if (!(t % prime == 0))
            {
                write(p[n + 1][1], &t, sizeof(int));
            }
        }
        close(p[n][0]);
        close(p[n + 1][1]);
        wait(0); // Reap the child
        exit(0);
    }
}

int main()
{
    pipe(p[0]);
    int pid = fork();
    if (pid == 0)
    {
        primes(0);
    }
    else
    {
        close(p[0][0]);
        for (int start = 2; start <= 280; start++)
        {
            write(p[0][1], &start, sizeof(int));
        }
        close(p[0][1]);
    }
    wait(0);
    exit(0);
}