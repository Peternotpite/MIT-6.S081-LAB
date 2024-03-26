#include"kernel/types.h"
#include"user/user.h"
#include"kernel/param.h"
#define MAX_LEN 1024
#define stdin 0
#define stdout 1

int main(int argc, char *argv[]) 
//argc 变量是用来存储命令行参数的数量的
//argv 的值取决于命令行中传递给shell的参数数量以及各个参数的值。

{
    //数据结构 
    int pid,n,index=0;
    char buf,*args[MAXARG],arg[MAX_LEN];

    //读取argv的参数
    for(int i=1 ; i<argc ; i++)
        args[i-1] = argv[i]; //argv[0] 为程序的名称或路径，通常是shell的名称或路径。

    //读取stadin的参数
    while((n = read(stdin , &buf , 1)) > 0)
    {
        if(buf == '\n')
        {
            arg[index] = 0;
            if((pid = fork()) < 0)
            {
                fprintf(stdout, "Error , fork workout !");
            }else if(pid == 0) //child process
            {
                args[argc-1] = arg;
                args[argc] = 0;
                exec(args[0],args);
            }else //father process
            {
                wait(0);
                index = 0;
            }
        }else {
            arg[index++] = buf;
        }
        
    }
    exit(0);
}