#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void handle(union sigval v)
{
    time_t t;
    char p[32];

    time(&t);
    strftime(p, sizeof(p), "%T", localtime(&t));

    printf("%s thread %lu, val = %d, signal captured.\n", p, pthread_self(), v.sival_int);

    return;
}


int main( int argc, char **argv ) {

    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;

    memset (&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = &timer; //句柄
    evp.sigev_notify = SIGEV_THREAD; //设置通知方式,通知方式可以有不通知,用信号通知,用 invoking sigev_notify_function 通知
    evp.sigev_notify_function = handle; //设置处理函数
    evp.sigev_value.sival_int = 3;  //作为handle()的参数

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if( ret){
        perror("timer_create");
        exit(-1);    
    }

    ts.it_interval.tv_sec = 1; // 第一次触发后的 时间间隔 秒
    ts.it_interval.tv_nsec = 0; // 时间间隔 纳秒
    ts.it_value.tv_sec = 1; //第一次触发时间 秒
    ts.it_value.tv_nsec = 0;  //第一次触发事件 纳秒

    ret = timer_settime(timer, /*TIMER_ABSTIME*/0, &ts, NULL);
    if( ret ){
        perror("timer_settime");
        exit(-1);    
    }

    sleep(30);
    return 0;
}
