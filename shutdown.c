#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPiSPI.h>

#define ZERO 0 // channel zero
#define ONE 1  // channel one

uint8_t buf[2];

void spi(int channel, uint8_t reg, uint8_t val) {
    buf[0] = reg;
    buf[1] = val;
    /*printf("before buf zero %x\n", buf[0]);*/
    wiringPiSPIDataRW(channel, buf, 2);
    /*printf("buf zero %x\n", buf[0]);*/
    /*printf("buf one %x\n", buf[1]);*/
    /*usleep(50);*/
}

void shutdown() {
    spi(ZERO, 0x0C, 0x00);
    spi(ONE, 0x0C, 0x00);
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1

        */

	if (wiringPiSPISetup(ZERO, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}
        if (wiringPiSPISetup(ONE, 1000000) < 0) {
                fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
                exit(errno);
        }

        shutdown();
}


