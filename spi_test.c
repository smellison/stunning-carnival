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
    printf(buf);
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

        TODO test bcd mode just to see if that's somehow functioning?
        */

	if (wiringPiSPISetup(CHANNEL, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}

        /* set bcd */
        spi(0x09, 0x0F);

        /* set scan limit */
        spi(0x0B, 0x01);

        /* set intensity */
        spi(0x0A, 0x0C);

        /* normal operation mode, after setup */
        spi(0x0C, 0x01);

        /* send some test data */
        spi(0x01, 0x03);

        sleep(10);

        shutdown();
/*
	for (;;) {
                spi(0xFF, 0xFF);
		sleep(1);
                spi(0xFF, 0x00);
		sleep(0);
	}*/
}


