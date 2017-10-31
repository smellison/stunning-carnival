#include "stats.h"

#include <stdio.h> /* printf, sprintf */
#include <unistd.h>

int main()
{
	while(1) {
		char *result = make_request();
		printf("%s\n",result);
		fflush(stdout);
		sleep(20);
	}
    return 0;
}
