#include "mbed.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
Thread thread1, thread2, thread3;


void led1_thread()
{
    while (true) {
        led1 = !led1;
        Thread::wait(500);
    }
}

void led2_thread()
{
    while (true) {
        led2 = !led2;
        ThisThread::sleep_for(1000);
    }
}

void led3_thread()
{
    while (true) {
        led3 = !led3;
        Thread::wait(250);
    }
}

int main()
{
    
    thread1.start(callback(led1_thread));
    thread2.start(callback(led2_thread));
    thread3.start(callback(led3_thread));

    while (true) {
        Thread::wait(1000);
    }
}