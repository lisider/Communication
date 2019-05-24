/*
 * 该文件实现了一个 互斥锁 对 两个线程的互斥
 * 两个线程 分别为两个模块
 * 执行顺序不确定,但两个模块不会同时执行
 *
 * */
#include <pthread.h>

#include <stdio.h>
#include <unistd.h>

pthread_t       pthid1,pthid2;

int i = 1000;

static void * fun1(void *arg){
    while(1){

        printf("thread1 --- %d\n",--i);
        usleep(1000);
        printf("thread1 --- %d\n",++i);

    }
    return NULL;
}

static void * fun2(void *arg){
    while(1){

        printf("thread2 --- %d\n",--i);
        usleep(1000);
        printf("thread2 --- %d\n",++i);

    }
    return NULL;
}


int main(int argc, const char *argv[])
{

    int arg1 = 5;
    int *retval;



    if(0 != pthread_create(&pthid1,NULL,fun1,NULL)){
        perror("pthid1");
        return -1;
    }
    if(0 != pthread_create(&pthid2,NULL,fun2,NULL)){
        perror("pthid2");
        return -1;
    }

    pthread_join(pthid1,(void **)&retval);
    //printf("the exit status of pthid2 is %d\n",*retval);
    pthread_join(pthid2,NULL);

    return 0;
}
