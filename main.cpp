#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"


C12832 lcd(p5, p7, p6, p8, p11);
LM75B temp_sensor(p28,p27);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
Thread thread1, thread2, thread3;

// Create the local filesystem 
LocalFileSystem local("local");

const int DATA_SIZE = 100;
const int LOG_INTERVAL_SEC = 300; // log data every 5 minutes
const char* FILENAME = "/local/data.txt";

float data[DATA_SIZE];
int data_count = 0;


void led1_thread()
{
        float get_min(float *data, int size) {
        float min = data[0];
        for (int i = 1; i < size; i++) {
         if (data[i] < min) {
            min = data[i];
    }
  }
  return min;
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