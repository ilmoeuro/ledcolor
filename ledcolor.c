/*
MIT License

Copyright (c) 2018 Ilmo Euro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "wiringPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void usage(char *argv0) {
    fprintf(stderr, "Usage: %s <PIN> [0-255] [0-255] [0-255]\n", argv0);
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 5) {
        usage(argv[0]);
    }

    int pin;
    int vals[3];
    struct timespec sleep_time;
    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 0;

    if (sscanf(argv[1], "%d", &pin) != 1) {
        usage(argv[0]);
    }

    for (int i=0; i<3; i++) {
        if (sscanf(argv[2 + i], "%d", &vals[i]) != 1) {
            usage(argv[0]);
        }

        if (vals[i] < 0 || vals[i] > 255) {
            usage(argv[0]);
        }
    }

    wiringPiSetup();
    pinMode(pin, OUTPUT);

    for (int j = 0; j < 3; j++) {
        int val = vals[j];
        int on_time;
        int off_time;

        for (int i=7; i >= 0; i--) {
            if (val & (1<<i)) {
                on_time = 1200;
                off_time = 1300;
            } else {
                on_time = 500;
                off_time = 2000;
            }
            digitalWrite(pin, 1);
            sleep_time.tv_nsec = on_time;
            nanosleep(&sleep_time, NULL);
            digitalWrite(pin, 0);
            sleep_time.tv_nsec = off_time;
            nanosleep(&sleep_time, NULL);
        }
    }

    return 0;
}