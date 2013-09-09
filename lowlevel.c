#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "lowlevel.h"

/** Set Raspberry Pi Board version */
// #define RPI_REV1
#ifndef RPI_REV1
#define I2C_BUS "/dev/i2c-1"
#else
#define I2C_BUS "/dev/i2c-0"
#endif

/** The address for the expansion board */
#define I2C_ADDR                0x4E

/** File descriptor for the I2C bus */
static int fd;

/** Initialisation status */
static int g_init = 0;

/**
 * @brief Write to the weather station board.
 * @return the same as the write call
 */
int exp_write(const uint8_t *buf, int len)
{
    if (!g_init) {
        fprintf(stderr, "exp_write error: is not initialised.\n");
        return -1;
    }

    int r = write(fd, buf, len);
    if (r < 0) {
        fprintf(stderr, "exp_write error: %s\n", strerror(errno));
    }
    usleep(1000);
    return r;
}

/**
 * @brief read from the weather station board
 * @return the same as the read call
 */
int exp_read(uint8_t* buf, int len)
{
    if (!g_init) {
        fprintf(stderr, "exp_read error: GPIO not initialised.\n");
        return -1;
    }

    int r = write(fd, buf, len);
    if (r < 0) {
        fprintf(stderr, "exp_write error: %s\n", strerror(errno));
    }
    usleep(1000);
    return r;
}

int exp_open()
{
    if (g_init == 1) {
        fprintf(stderr,
                "GPIO_open: I/O expander is already initialised.\n");
        return 0;
    }
    g_init = 1;
    fd = open(I2C_BUS, O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "GPIO_open error: %s\n", strerror(errno));
        return -1;
    }
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) == -1) {
        fprintf(stderr, "GPIO_open: ioctl error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int exp_close()
{
    int t = close(fd);
    if (t == 0) {
        fd = g_init = 0;
    } else {
        fprintf(stderr, "GPIO_close error: %s\n", strerror(errno));
    }
    return t;
}
