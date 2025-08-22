/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 16:20:39
 * @LastEditTime : 2025-08-22 16:46:07
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : xargs.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define NULL 0

char *new_argv[128];
char buf[1024];

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        new_argv[i - 1] = argv[i]; // Copy the given argv
    }
    while (strlen(gets(buf, 1000)) > 0)
    {
        int i = 0;
        int new_argc = argc - 1;
        while (buf[i] != 0)
        {
            while (buf[i] == ' ')
            {
                buf[i] = 0;
                i++;
            }
            new_argv[new_argc] = &buf[i];
            new_argc++;
            while (buf[i] != ' ' && buf[i] != 0)
            {
                if (buf[i] == '\n' || buf[i] == '\r')
                {
                    buf[i] = 0;
                }
                i++;
            }
        }
        new_argv[new_argc] = NULL;
        int pid = fork();
        if (pid == 0)
        {
            exec(argv[1], new_argv);
        }
        else
        {
            wait(0);
        }
    }
}