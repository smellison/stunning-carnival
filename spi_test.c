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
    wiringPiSPIDataRW(ZERO, buf, 2);
}

void bootone() {
	if (wiringPiSPISetup(ZERO, 1000000) < 0) {
		fprintf (stderr, "SPI Setup failed: %s\n", strerror(errno));
		exit(errno);
	}

        /* set decode 11111111 */
        spi(0x01, 0xFF);

        /* set global intensity to a medium value */
        spi(0x02, 0x07);

        /* set scan limit to one digit 0x00, six is 0x05 */
        spi(0x03, 0x00);

        /* control register PIRTEBxS
	P - Blink Phase Readback 0 is p1 blink phase, 1 is p0 blink phase
	I - Global Intensity, 0 global intensity via one setting, 1 individual intensity settings
	R - Global Clear Digit Data, 0 digit data for both planes p0 and p1 are unaffected
	T - Global Blink Timing Synchronization, 0 Blink timing counters are unaffected, 1 counters are reset on rising edge of CS
	E - Global Blink Enable/Disable, 0 is off, 1 is on
	B - Blink Rate Selection 0 is 1s/1s, 1 is .5s/.5s duty cycles
        x - Don't Care Bit
	S - Shutdown control 0 is shutdown, 1 is normal op
	00000001 
	so, just turn it on!
	*/
        spi(0x04, 0x01);

        /* GPIO data - Do Nothing */

        /* Port Configuration - Do Nothing I guess */
        /* Display Test, off */
	spi(0x07, 0x00);

        /* 
	Everything else:
	Key masks
	Digit Type
	Intesities
	Digit 0 - 7a 0x60 -> 0x6F
	Keys Debounced
	Keys Pressed
	*/
}

void shutdown() {
    spi(0x04, 0x00);
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1

        */

	bootone();

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

        /* turn on display test */
        /* spi(0x07, 0x01); */

	/* turn on digit 0, plane p0 with 0100 0010 */
        spi(0x20,0x42);

	sleep(2);

        /* B? */
	spi(0x20,0x4D);

}


