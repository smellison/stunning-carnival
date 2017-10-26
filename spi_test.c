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
    wiringPiSPIDataRW(channel, buf, 2);
}

void bootone() {
	if (wiringPiSPISetup(ZERO, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}

        /* set bcd OFF */
        spi(ZERO, 0x09, 0x00);

        /* set scan limit to one digit for now */
        spi(ZERO, 0x0B, 0x07);

        /* set intensity */
        spi(ZERO, 0x0A, 0x0C);

        /* normal operation mode, after setup */
        spi(ZERO, 0x0C, 0x01);
}

void boottwo() {
        if (wiringPiSPISetup(ONE, 1000000) < 0) {
                fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
                exit(errno);
        }

        /* set bcd OFF */
        spi(ONE, 0x09, 0x00);

        /* set scan limit to one digit for now */
        spi(ONE, 0x0B, 0x07);

        /* set intensity */
        spi(ONE, 0x0A, 0x0C);

        /* normal operation mode, after setup */
        spi(ONE, 0x0C, 0x01);
}

void shutdown() {
    spi(ZERO, 0x0C, 0x00);
    spi(ONE, 0x0C, 0x00);
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1

        */

	bootone();
	boottwo();

        usleep(200);

        /* trying to set all segments on for digit 1.
	for each digit (which is actually now the segment) set it high
        this makes sense but shows nothing
	for common cathode, the digit pin must be held high until the digit is active when it goes low
	so under the normal setup, digit 0 to display all segments would set digit 0 pin to low, and then turn on all segments supplying current. 
	now we've reversed that, and we connect our segment A to digit 0's anode (which would go high when active as desired)
        Then each segment is connected to the digits and we must one by one set them to the target value
	this is done by cycling through each digit and setting the relevant segment on or off
	Instead of reading the binary values for each digit horizontally, we must know think of them vertically where we read the bit at position[digit] for each address (segment)
	 */

    for(;;) {
        /* turn on zero */
        spi(ZERO, 0x0C, 0x01);

        spi(ZERO, 0x01, 0x60);
        spi(ZERO, 0x02, 0x60);
        spi(ZERO, 0x03, 0x60);
        spi(ZERO, 0x04, 0x60);
        spi(ZERO, 0x05, 0x60);
        spi(ZERO, 0x06, 0x60);
        spi(ZERO, 0x07, 0x60);
        spi(ZERO, 0x08, 0x60);

	/* shutdown zero */
        spi(ZERO, 0x0C, 0x00);

        /*usleep(1);*/

        /* turn on one */
        spi(ONE, 0x0C, 0x01);

        spi(ONE, 0x01, 0x00);
        spi(ONE, 0x02, 0x00);
        spi(ONE, 0x03, 0x00);
        spi(ONE, 0x04, 0x00);
        spi(ONE, 0x05, 0x40);
        spi(ONE, 0x06, 0x00);
        spi(ONE, 0x07, 0x00);
        spi(ONE, 0x08, 0x00);
       
        /* shutdown one */
        spi(ONE, 0x0C, 0x00);

        /*usleep(1);*/
    }

}


