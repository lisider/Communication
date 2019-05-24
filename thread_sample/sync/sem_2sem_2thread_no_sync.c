/*
 *这个文件实现了两个信号量对于两个线程中的同步, 实现的是 循环同步
 * fun1 和 fun2 分别是两个模块
 * 同步顺序 是 fun1 fun2
 *
 *
 * */
#include <semaphore.h>

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_t   pthid1,pthid2;

int i = 1000;

static void * fun1(void *arg){
    while(1){

        printf("thread1 --- %d\n",--i);
        usleep(100000);

    }
    return NULL;
}

static void * fun2(void *arg){
    while(1){

        printf("thread2 --- %d\n",++i);
        usleep(100000);

    }
    return NULL;
}


int main(int argc, const char *argv[])
{


    if(0 != pthread_create(&pthid1,NULL,fun1,NULL)){
        perror("pthid1");
        return -1;
    }
    if(0 != pthread_create(&pthid2,NULL,fun2,NULL)){
        perror("pthid2");
        return -1;
    }

    pthread_join(pthid1,NULL);
    pthread_join(pthid2,NULL);

    return 0;
}
