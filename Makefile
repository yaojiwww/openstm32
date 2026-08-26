CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -pedantic -O2

APP_SOURCES = \
	Firmware/App/button.c \
	Firmware/App/config.c \
	Firmware/App/fault.c \
	Firmware/App/filter.c \
	Firmware/App/protocol.c \
	Firmware/App/sampler.c \
	Firmware/App/workstation.c

INCLUDES = -IFirmware/App -IFirmware/Port

.PHONY: test clean

test: Tests/test_runner
	./Tests/test_runner

Tests/test_runner: Tests/test_main.c $(APP_SOURCES)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	$(RM) Tests/test_runner
