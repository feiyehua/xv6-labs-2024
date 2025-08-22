/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 17:35:55
 * @LastEditTime : 2025-08-23 01:11:47
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : attack.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

char* expand_memory()
{
  char *end = sbrk(PGSIZE * 64);
  // printf("%p", end);
  // end += PGSIZE;
  // printf("%p", end);
  return end;
}

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  char *end=expand_memory();
  write(2, end + 0x10020, 8);
  exit(1);
}

/*
Add this line to secret.c,
  strcpy(end + 50, "1145141919810, mitxv6");

We can find the memory like this:
00010000: 0090 f287 0000 0000 7665 7279 2076 6572  ........very ver
00010010: 7920 7365 6372 6574 2070 7720 6973 3a20  y secret pw is:
00010020: 622e 622f 6165 6100 0000 0000 0000 0000  b.b/aea.........
00010030: 0000 3131 3435 3134 3139 3139 3831 302c  ..1145141919810,
00010040: 206d 6974 7876 3600 0000 0000 0000 0000   mitxv6.........

Therefore we can determine the location of the secret is enf + 0x10020.

*/