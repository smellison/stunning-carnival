#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPiSPI.h>

#define ZERO 0 // channel zero

uint8_t buf[2];

void spi(uint8_t reg, uint8_t val) {
    buf[0] = reg;
    buf[1] = val;
    /*printf("before buf zero %x\n", buf[0]);*/
    wiringPiSPIDataRW(ZERO, buf, 2);
    /*printf("buf zero %x\n", buf[0]);*/
    /*printf("buf one %x\n", buf[1]);*/
    /*usleep(50);*/
}

void shutdown() {
    spi(0x07, 0x00);
    spi(0x04, 0x00);
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1

        */

	if (wiringPiSPISetup(ZERO, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}
        usleep(200);
        shutdown();
}


