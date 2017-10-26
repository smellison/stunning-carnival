#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPiSPI.h>

#define CHANNEL 0

uint8_t buf[2];

void spi(uint8_t reg, uint8_t val) {
    buf[0] = reg;
    buf[1] = val;
    /*printf("before buf zero %x\n", buf[0]);*/
    wiringPiSPIDataRW(CHANNEL, buf, 2);
    printf("buf zero %x\n", buf[0]);
    printf("buf one %x\n", buf[1]);
    usleep(20);
}

void shutdown() {
    spi(0x0C, 0x00);
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1
        Digit 1 is selected via 0xX2 and so on
        the number 3 is written as 01111001 or 79

        H is 37
        I is 30
        S is 5B

        TODO solve random failure issue. Caps? resistors? sleeps?
        */

        /* send some test data */
        /*spi(0x01, 0x03);*/
        spi(0x01, 0x30);
        spi(0x02, 0x5B);

        sleep(5);

        /* blank my test data */
        spi(0x01, 0x00);
        spi(0x02, 0x00);
/*
        shutdown();
*/
}


