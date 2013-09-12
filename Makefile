ifeq ($(shell uname -m),armv6l)
  CC = cc
else
  CC = arm-linux-gnueabi-gcc-4.7
endif

CFLAGS = -std=gnu99 -Wall -Wextra -O2 -pedantic

ifneq ($(RPI_REV1),)
CFLAGS += -DRPI_REV1
endif

LDFLAGS =
COMMON_SOURCES = lowlevel.c
COMMON_OBJECTS = $(COMMON_SOURCES:.c=.o)
INSTALL_PATH = /usr/local/bin
EXECUTABLE = Pi-Water Pi-Sensors Pi-UV Pi-Time

all: $(EXECUTABLE)

Pi-Time: Pi-Time.o time.o $(COMMON_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

Pi-Water: Pi-Water.o darlington.o $(COMMON_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

Pi-UV: Pi-UV.o darlington.o $(COMMON_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

Pi-Sensors : Pi-Sensors.o sensors.o $(COMMON_OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

depend: .depend
.depend: *.c
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF  ./.depend
include .depend

.PHONY: clean
clean:
	rm .depend *.o $(EXECUTABLE) -rf

.PHONY: install
install:
	install -m 0755 $(EXECUTABLE) /usr/local/bin

.PHONY: uninstall
uninstall:
	rm -rf /usr/local/bin/$(EXECUTABLE)
