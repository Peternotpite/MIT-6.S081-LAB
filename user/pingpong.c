#include"kernel/types.h"
#include"user/user.h"

int p[2];
#define WRITE 0
#define READ 1
#define PIPE_READ p[0]
#define PIPE_WRITE p[1]

int main(int argc,char *argv[])
{
    int pid;
    //char buf[10];
    pipe(p);
    if(fork() == 0) //子进程
    {
        //读取父进程的pid
        if((read(PIPE_READ,&pid,1)) < 0)
        {
            fprintf(2,"Error, child_process don't get pid\n");
            exit(0);
        }
        close(PIPE_READ); 
        //子进程开始pingpong
        fprintf(1,"%d: recieved ping\n",pid);
        //将子进程的pid写入管道
        pid = getpid();
        if((write(PIPE_WRITE,&pid,1)) < 0)
        {
            fprintf(2,"Error, child_process don't write pid\n");
            exit(0);
        }
        close(PIPE_WRITE); 
        exit(0);
    }else{ //父进程
        pid = getpid();
        //将父进程的pid写入管道
        if((write(PIPE_WRITE,&pid,1)) < 0)
        {
            fprintf(2,"Error, father_process don't write pid\n");
            exit(0);
        }
        close(PIPE_WRITE);
        wait(0);
        //读取子进程的pid
        if((read(PIPE_READ,&pid,1)) < 0)
        {
            fprintf(2,"Error, father_process don't get pid\n");
            exit(0);
        }
        close(PIPE_READ);
        fprintf(1,"%d: recieved pong\n",pid);
        exit(0);
    }
}