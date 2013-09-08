#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lowlevel.h"
#include "darlington.h"

int main(int argc, const char *argv[])
{
    if (argc == 1) {
        fprintf(stderr,
                "Usage: Pi-Water TIME\nThe unit for TIME is in seconds.\n" );
        exit(-1);
    }
    int sec = atoi(argv[1]);
    if (sec == 0) {
        fprintf(stderr,
                "Please supply a valid number.\n" );
        exit(-2);
    }
    printf("Turing on the pump for %d seconds...", sec);
    exp_open();
    int r = 0;
    r += darlington_on(1);
    r += darlington_on(2);
    r += darlington_on(3);
    r += darlington_on(4);
    sleep(sec);
    r += darlington_off(1);
    r += darlington_off(2);
    r += darlington_off(3);
    r += darlington_off(4);
    exp_close();
    printf("Done!\n");
    return r;
}
