#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <time.h>

typedef void (*func_t)(void);

timer_t timer;

void func(void){
    printf("func\n");
}

void handle(union sigval v)
{
    time_t t;
    char p[32];
    struct itimerspec value;
    func_t fun = NULL;

    time(&t);
    strftime(p, sizeof(p), "%T", localtime(&t));

    //获取剩余时间
    timer_gettime(timer,&value);

    printf("handle begin +++\n");

    printf("left time to next handle : second,%lu,nanosecond,%lu\n",value.it_value.tv_sec,value.it_value.tv_nsec);

    printf("current time : %s\n", p);
    printf("current thread %lu\n",pthread_self());

    printf("call sival_int : %d\n",v.sival_int);

    fun = v.sival_ptr;
    printf("call sival_ptr : ");
    fun();

    printf("handle end ---\n");

    return;
}



int main( int argc, char **argv ) {

    struct sigevent evp;
    struct itimerspec ts;
    int ret;

    // 1. 创建一个定时器

    memset (&evp, 0, sizeof(evp));
    evp.sigev_notify = SIGEV_THREAD; //设置通知方式,通知方式可以有不通知,用信号通知,用 invoking sigev_notify_function 通知
    evp.sigev_notify_function = handle; //设置处理函数

    // 设置两个 sigev_notify_function 的参数
    evp.sigev_value.sival_int = 3;  //作为handle()的参数
    evp.sigev_value.sival_ptr = func;  //作为handle()的参数

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if( ret){
        perror("timer_create");
        exit(-1);
    }

    // 2. 启动一个定时器

    // 第一次触发后到第二次触发的间隔
    // 第2次触发后到第3次触发的间隔
    ts.it_interval.tv_sec = 1; // 第一次触发后的 时间间隔 秒
    ts.it_interval.tv_nsec = 0; // 时间间隔 纳秒

    // 开启timer后到第一次触发的间隔
    // 下面的 赋值 不可全 为 0
    ts.it_value.tv_sec = 1; //第一次触发时间 秒
    ts.it_value.tv_nsec = 0;  //第一次触发事件 纳秒

    ret = timer_settime(timer, /*TIMER_ABSTIME*/0, &ts, NULL);
    if( ret ){
        perror("timer_settime");
        exit(-1);
    }

    sleep(5);

    int timers = timer_getoverrun(timer);
    printf("times:%d\n",timers);

    // 3. 删除一个定时器
    timer_delete(timer);
    return 0;
}
