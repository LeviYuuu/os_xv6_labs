#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int subProcess(int *oldFd)
{
    close(oldFd[1]);
    int fd[2];
    int prime;
    int num;
    if (read(oldFd[0], &prime, 4)) 
    {
        printf("prime %d\n", prime); 
        pipe(fd); 
        if (fork() == 0) 
            subProcess(fd); 
        else                         // 父进程
        {
            close(fd[0]);
            while (read(oldFd[0], &num, 4)) // 从原管道进行读
            {
                if (num % prime != 0) 
                    write(fd[1], &num, 4);
            }
            close(oldFd[0]); // 此时父进程的原管道关闭, 则关闭原管道的读端
            close(fd[1]); 
            wait((int *)0); 
        }
    }
    else
        close(oldFd[0]); // 此时说明原管道已关闭
    exit(0);
}
int main()
{
    int fd[2];
    pipe(fd);
    if (fork() == 0) 
        subProcess(fd);
    else // 父进程
    {
        close(fd[0]);
        for (int i = 2; i <= 35; ++i) 
            write(fd[1], &i, 4);
        close(fd[1]); 
        wait((int *)0);
    }
    exit(0);
}
