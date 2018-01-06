/*
 * 这个文件实现了四个线程的互斥,
 * 读和读不互斥
 * 读和写互斥
 * 写和写互斥
 *
 * */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_rwlock_t lock;

void * write1(void *arg)
{
    pthread_rwlock_wrlock(&lock);
    puts("write1");
    puts("write1");
    puts("write1");
    puts("write1");
    pthread_rwlock_unlock(&lock);

    return((void *)1);
}

void *write2(void *arg)
{
    pthread_rwlock_wrlock(&lock);
    puts("write2");
    puts("write2");
    puts("write2");
    puts("write2");
    pthread_rwlock_unlock(&lock);

    return((void *)2);
}

void *read1(void *arg)
{
    pthread_rwlock_rdlock(&lock);
    puts("read1");
    puts("read1");
    puts("read1");
    puts("read1");
    pthread_rwlock_unlock(&lock);
    return((void *)3);
}

void *read2(void *arg)
{
    pthread_rwlock_rdlock(&lock);
    puts("read2");
    puts("read2");
    puts("read2");
    puts("read2");
    pthread_rwlock_unlock(&lock);
    return((void *)3);
}

int main(void)
{
    pthread_t   th1,th2,th3,th4;
    int         ret;
    ret =pthread_create(&th1,NULL,write1,NULL);
    if( ret != 0 )
    {
        perror("pthread1 ");
        return 0;
    }
    ret = pthread_create(&th3,NULL,read1,NULL);
    if( ret != 0 )
    {
        perror("pthread3 ");
        return 0;
    }
    ret = pthread_create(&th4,NULL,read2,NULL);
    if( ret != 0 )
    {
        perror("pthread4 ");
        return 0;
    }
    ret = pthread_create(&th2,NULL,write2,NULL);
    if( ret != 0 )
    {
        perror("pthread2 ");
        return 0;
    }
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    pthread_join(th4,NULL);
    pthread_rwlock_destroy(&lock);
    return 1;
}
