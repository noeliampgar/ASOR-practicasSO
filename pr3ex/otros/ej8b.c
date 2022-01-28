#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <linux/limits.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Falta argumentos\n");
        exit(1);
    }
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        pid_t sesion = setsid();
        if (sesion == -1)
        {
            perror("setsid");
            exit(1);
        }
        int fd1 = open("/tmp/daemon.out", O_CREAT | O_RDWR, 00777);
        if (fd1 == -1)
        {
            perror("open");
            exit(1);
        }
        int fd2 = open("/tmp/daemon.err", O_CREAT | O_RDWR, 00777);
        if (fd2 == -1)
        {
            perror("open");
            exit(1);
        }
        int fd3 = open("/dev/null", O_CREAT | O_RDWR, 00777);
        if (fd3 == -1)
        {
            perror("open");
            exit(1);
        }
        int d1 = dup2(fd1, 1);
        if (d1 == -1)
        {
            perror("dup2");
            exit(1);
        }
        int d2 = dup2(fd2, 2);
        if (d2 == -1)
        {
            perror("dup2");
            exit(1);
        }
        int d3 = dup2(fd3, 0);
        if (d3 == -1)
        {
            perror("dup2");
            exit(1);
        }
        int val = execvp(argv[1], argv + 1);
        if (val == -1)
        {
            perror("execvp");
            exit(1);
        }
        exit(0);
    }
    else
    {
        sleep(3);
    }
    return 0;
}