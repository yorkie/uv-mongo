CC     ?= gcc
SRC     = $(wildcard src/*.c) tests/simple.c
DEPS    = $(wildcard deps/*/*.c)
CFLAGS  = -std=c99\
					-Isrc\
					-Ideps\
					-Ideps/libuv/include\
					-I/usr/local/include/node\
					-Wall -Wno-unused-function
LDFLAGS = -lcrypto\
					-lssl\
					deps/libuv/libuv.a\
					-framework CoreFoundation -framework CoreServices

test: $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(DEPS) $(LDFLAGS)

.PHONY: test