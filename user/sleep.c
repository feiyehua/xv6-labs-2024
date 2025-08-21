/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 00:31:00
 * @LastEditTime : 2025-08-22 01:01:31
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : sleep.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

    if (argc <= 1)
    {
        write(1, "usage: sleep number\n", 20);
        exit(0);
    }
    int i = atoi(argv[1]);
    sleep(i);
    exit(0);
}
