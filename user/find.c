/*
 * @Author       : FeiYehua
 * @Date         : 2025-08-22 13:24:37
 * @LastEditTime : 2025-08-22 16:18:57
 * @LastEditors  : FeiYehua
 * @Description  :
 * @FilePath     : find.c
 *      © 2024 FeiYehua
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char name[512];

char *fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void ls(char *path)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (st.type != T_DIR)
    {
        if (strcmp(name, fmtname(path)) == 0)
        {
            printf("%s\n", buf);
        }
        return;
    }
    else
    {
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            exit(0);
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if (st.type != T_DIR && strcmp(de.name, name) == 0)
            {
                printf("%s\n", buf);
            }
            else if (st.type == T_DIR && !(strcmp(de.name, ".") == 0) && !(strcmp(de.name, "..") == 0))
            {
                ls(buf);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(1, "usage: find path [expression]");
        exit(0);
    }
    strcpy(name, argv[2]); // Copy the specified name to a buffer
    ls(argv[1]);
}