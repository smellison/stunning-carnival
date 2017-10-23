#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPiSPI.h>

#define CHANNEL 0

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1
        Digit 1 is selected via 0xX2 and so on
        the number 3 is written as 01111001 or 79
 
        TODO test bcd mode just to see if that's somehow functioning?
        */

	/*uint8_t on[] = { 0x01, 0x79 };*/
        /*uint8_t on[] = { 0xFF, 0xFF };*/
        uint8_t on[] = { 0x01, 0x03 };
	uint8_t off[] = { 0xFF, 0x00 };
	uint8_t buf[2];

	if (wiringPiSPISetup(CHANNEL, 8000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}
        
        uint8_t intensity = { 0x0A, 0x05 };
        uint8_t scan_limit = { 0x0B, 0x01 }; 
        uint8_t shutdown = { 0x0C, 0x01 };
        uint8_t bcd = { 0x09, 0x00 };

        memcpy(buf, bcd, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);

        memcpy(buf, scan_limit, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);

        memcpy(buf, intensity, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);

        memcpy(buf, shutdown, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);

        memcpy(buf, on, 2);
        wiringPiSPIDataRW(CHANNEL, buf, 2);
/*
	for (;;) {
		memcpy(buf, on, 2);
		wiringPiSPIDataRW(CHANNEL, buf, 2);
		sleep(1);
		memcpy(buf, off, 2);
		wiringPiSPIDataRW(CHANNEL, buf, 2);
		sleep(0);
	}*/
}
