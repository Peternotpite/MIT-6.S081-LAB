#include"kernel/types.h"
#include"user/user.h"

#define MAXLEN 35
#define READ 1
#define WRITE 0
void primes(int *p);

int main()
{
    int p_parent[2];
    pipe(p_parent);

    if(fork() == 0) //child process
    {
        primes(p_parent);
    }else{ //parent process
        close(p_parent[WRITE]); //close p_parent process read
        for(int i=2;i<=MAXLEN;i++)
        {
            if(write(p_parent[READ],&i,sizeof(int)) < 0)
            {
                fprintf(2,"Error, p_parent process can't write number!\n");
                exit(0);
            }
        }
        close(p_parent[READ]);
        wait(0);
    }
    exit(0);
}

void primes(int *p)
{
    int buf,prime;
    int p_child[2];
    pipe(p_child);
    close(p[READ]); //close parent process write
    read(p[WRITE],&buf,sizeof(int));
    if(buf == 0) //end of prime numbers
    {
        exit(0);
    }
    prime = buf;
    if(fork() == 0) //child process
    {
        primes(p_child);
    }else{ //parent process
        close(p_child[WRITE]); //close p_child process read
        fprintf(1,"prime %d\n",prime);
        while(read(p[WRITE],&buf,sizeof(int)))
        {
            if((buf % prime) != 0)
            {
                if(write(p_child[READ],&buf,sizeof(int)) < 0)
                {
                    fprintf(2,"Error, p_child process can't write number!\n");
                    exit(0);
                }
            }
        }
        close(p[WRITE]); //close parent process read 
        close(p_child[READ]); //close p_child process write
        wait(0);
        exit(0);
    }
}