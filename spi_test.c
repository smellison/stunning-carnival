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
    wiringPiSPIDataRW(CHANNEL, buf, 2);
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

        TODO solve random failure issue. Caps? resistors? sleeps?
        */

	if (wiringPiSPISetup(CHANNEL, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}

        /* set bcd OFF */
        spi(0x09, 0x00);

        /* set scan limit */
        spi(0x0B, 0x01);

        /* set intensity */
        spi(0x0A, 0x0C);

        /* normal operation mode, after setup */
        spi(0x0C, 0x01);

        /* send some test data */
        /*spi(0x01, 0x03);*/
        spi(0x01, 0x37);
        spi(0x02, 0x30);

        sleep(10);

        /* blank my test data */
        spi(0x01, 0x00);
        spi(0x02, 0x00);

        shutdown();
/*
	for (;;) {
                spi(0xFF, 0xFF);
		sleep(1);
                spi(0xFF, 0x00);
		sleep(0);
	}*/
}


