#include "mbed.h"
#include "C12832.h"
#include "LM75B.h"

C12832 lcd(p5, p7, p6, p8, p11);
LM75B temp_sensor(p28, p27);

// Create the local filesystem
LocalFileSystem local("local");

const int DATA_SIZE = 100;
const int LOG_INTERVAL_SEC = 300; // log data every 5 minutes
const char* FILENAME = "/local/data.txt";

float data[DATA_SIZE];
int data_count = 0;
float min_temp = 0.0f, max_temp = 0.0f, avg_temp = 0.0f;

//Thread setup
Thread min_thread;
Thread max_thread;
Thread avg_thread;

// Calculations for the Min Data
void calculate_min() {
    while (1) {
        if (data_count > 0) {
            min_temp = data[0];
            for (int i = 1; i < data_count; i++) {
                if (data[i] < min_temp) {
                    min_temp = data[i];
                }
            }
        }
        Thread::wait(500);
    }
}

// Calculations for the Max Data
void calculate_max() {
    while (1) {
        if (data_count > 0) {
            max_temp = data[0];
            for (int i = 1; i < data_count; i++) {
                if (data[i] > max_temp) {
                    max_temp = data[i];
                }
            }
        }
        Thread::wait(500);
    }
}

// Calculations for the Average Data
void calculate_avg() {
    while (1) {
        if (data_count > 0) {
            float sum = 0.0f;
            for (int i = 0; i < data_count; i++) {
                sum += data[i];
            }
            avg_temp = sum / data_count;
        }
        Thread::wait(500);
    }
}

int main() {
    min_thread.start(callback(calculate_min));
    max_thread.start(callback(calculate_max));
    avg_thread.start(callback(calculate_avg));

    Timer log_timer;
    log_timer.start();

    while (1) {
        // check if it's time to log data
        if (log_timer.read() > LOG_INTERVAL_SEC) {
            log_timer.reset();

            //Temp measured
            float temp = temp_sensor.read();
            data[data_count] = temp;
            data_count++;

            //Storage of data
            FILE* fp = fopen(FILENAME, "a");
            if (fp != NULL) {
                fprintf(fp, "%.2f\n", temp);
                fclose(fp);
            }
        }

        //Files read from storage
        FILE* fp = fopen(FILENAME, "r");
        if (fp != NULL) {
            int i = 0;
            while (!feof(fp) && i < DATA_SIZE) {
                fscanf(fp, "%f", &data[i]);
                i++;
            }
            fclose(fp);
            data_count = i;

            // show temp data on LCD
            lcd.cls();
            lcd.locate(0,3);
            lcd.printf("Min temp: %.2fC\n", min_temp);
            lcd.printf("Max temp: %.2fC\n", max_temp);
            lcd.printf("Avg temp: %.2fC\n", avg_temp);
        }

        // wait for 1 second
        wait(1);
    }
}
