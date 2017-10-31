#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

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
        spi(0x03, 0x05);

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

}

void shutdown() {
    spi(0x04, 0x00);
}

uint8_t get_digit_addr(int current_index) {
	uint8_t addr;
	switch (current_index) {
		case 0:
			addr = 0x20;
			break;
		case 1:
			addr = 0x21;
			break;
		case 2:
			addr = 0x22;
			break;
		case 3:
			addr = 0x23;
			break;
		case 4:
			addr = 0x24;
			break;
		case 5:
			addr = 0x25;
			break;
		default:
			addr = 0x20;
	}
	return addr;
}

void print_seq(char chr){
	/* something like a for loop over each character, tracking position to know which char to print */
	int index;
	for(index=0; index < 8; index = index + 1) {
		uint8_t addr = get_digit_addr(index);
		switch(tolower(chr)) {
			case 'a':
				spi(addr,0x41);
				break;
			case 'b':
				spi(addr,0x42);
				break;
			case 'c':
				spi(addr,0x43);
				break;
			case 'd':
				spi(addr,0x44);
				break;
			case 'e':
				spi(addr,0x45);
				break;
			case 'f':
				spi(addr,0x46);
				break;
			case 'g':
				spi(addr,0x47);
				break;
			case 'h':
				spi(addr,0x48);
				break;
			case 'i':
				spi(addr,0x49);
				break;
			case 'j':
				spi(addr,0x4A);
				break;
			case 'k':
				spi(addr,0x4B);
				break;
			case 'l':
				spi(addr,0x4C);
				break;
			case 'm':
				spi(addr,0x4D);
				break;
			case 'n':
				spi(addr,0x4E);
				break;
			case 'o':
				spi(addr,0x4F);
				break;
			case 'p':
				spi(addr,0x50);
				break;
			case 'q':
				spi(addr,0x51);
				break;
			case 'r':
				spi(addr,0x52);
				break;
			case 's':
				spi(addr,0x53);
				break;
			case 't':
				spi(addr,0x54);
				break;
			case 'u':
				spi(addr,0x55);
				break;
			case 'v':
				spi(addr,0x56);
				break;
			case 'w':
				spi(addr,0x57);
				break;
			case 'x':
				spi(addr,0x58);
				break;
			case 'y':
				spi(addr,0x59);
				break;
			case 'z':
				spi(addr,0x5A);
				break;
			case '0':
				spi(addr,0x30);
				break;
			case '1':
				spi(addr,0x31);
				break;
			case '2':
				spi(addr,0x32);
				break;
			case '3':
				spi(addr,0x33);
				break;
			case '4':
				spi(addr,0x34);
				break;
			case '5':
				spi(addr,0x35);
				break;
			case '6':
				spi(addr,0x36);
				break;
			case '7':
				spi(addr,0x37);
				break;
			case '8':
				spi(addr,0x38);
				break;
			case '9':
				spi(addr,0x39);
				break;
			default:
				spi(addr,0x10); /* blank it */
		}
	}
}

void main(int argc, char** argv) {

        /*
        Digit 0 is selected via 0xX1

        */

	bootone();

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
        /*spi(0x07, 0x01); */
	/*sleep(5);*/

	/* turn on digit 0, plane p0 with 0100 0010 */
        /*spi(0x20,0x42);*/

	print_seq('a');
	sleep(1);
	print_seq('b');
	sleep(1);
	print_seq('C');
	sleep(1);
	print_seq('X');
	sleep(1);
	print_seq('r');
	sleep(1);
	print_seq('d');
	sleep(1);
	print_seq('e');
	sleep(1);
	print_seq('f');
	sleep(1);
	print_seq('g');
	sleep(1);
	print_seq('h');
	sleep(1);
	print_seq('i');
	sleep(1);
	print_seq('j');
	sleep(1);
	print_seq('k');
}


