#include"kernel/types.h"
#include"user/user.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(2, "Usage: sleep workout\n");
        exit(1);
    }

    // 调用sleep函数，等待指定的秒数
    sleep(atoi(argv[1]));
    exit(0);

}