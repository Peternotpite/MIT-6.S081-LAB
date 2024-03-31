#include"kernel/types.h"
#include"kernel/stat.h"
#include"user/user.h"
#include"kernel/fs.h"
#include"kernel/fcntl.h"

void find(char *path, const char *name);

int main(int argc, char *argv[])
{
    char* path = argv[1];
    char* name = argv[2];

    if(argc < 3)
    {
        fprintf(2, "Usage: lack of argument \n");
        exit(1);
    }
    // 调用函数查找文件
    find(path, name);
    return 0;
}

void find(char *path, const char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    while(read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if(de.inum == 0) continue;
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }
        switch(st.type)
        {
            case T_FILE:
                if(strcmp(name, de.name) == 0)
                    fprintf(1,"%s\n", buf);
                break;
            case T_DIR:
                if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
                {
                    fprintf(1,"find: path too long\n");
                    break;
                }
                if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                {
                    find(buf,name);
                }
                break;
        }

    }
    close(fd);
    return ;
}
