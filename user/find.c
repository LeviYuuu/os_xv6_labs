#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *pfilename(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--) ;
    p++;
    return p;
}
int find(char *path, char *filename)
{
    int fd;            
    char buf[512], *p; 
    struct stat st;
    struct dirent de; 
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "open fail%s\n", path);
        exit(1);
    }
    if (fstat(fd, &st) < 0) 
    {
        fprintf(2, "fstat fail%s\n", path);
        close(fd);
        exit(1);
    }
    switch (st.type)
    {
    case T_FILE:                                   // 常规文件
        if (0 == strcmp(pfilename(path), filename)) 
            fprintf(1, "%s\n", path); 
        break;
    case T_DIR:                
        strcpy(buf, path);  
        p = buf + strlen(buf); 
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0) 
                continue;
            if (0 == strcmp(".", de.name) || 0 == strcmp("..", de.name))
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, filename); // 递归
        }
        break;
    }
    close(fd); // 关闭文件描述符
    return 0;
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(2, "not enough arguments\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
