#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc != 1)
    {
        fprintf(2, "Error...\n");
        exit(1);
    }
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
        close(p[0]); 
        char temp = 'x';
        if (write(p[1], &temp, 1))
            fprintf(0, "%d: received ping\n", getpid());
        close(p[1]); 
    }
    else
    {
        wait((int *)0);
        close(p[1]); 
        char temp;
        if (read(p[0], &temp, 1))
            fprintf(0, "%d: received pong\n", getpid());
        close(p[0]); 
    }
    exit(0);
}
