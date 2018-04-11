/*
 *该文件 实现了一个信号对于两个线程的同步,其实信号可以同步两个进程
 *
 *
 * */

#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


void handler(int arg){
    printf("signal comes\n");
    return;
}

#define SLEEP(x) \
do{\
    int i = (x)*1000;\
    do{\
        usleep(1000);\
    }while(--i);\
}while(0)




int main(int argc, const char *argv[])
{
    signal(SIGALRM,handler);
    alarm(3);
    SLEEP(100);
    return 0;
}
