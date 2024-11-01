CFLAGS  := -std=c99 -Wall -g -O2 -D_REENTRANT
LIBS    := -lpthread -lm -lcrypto -lssl

TARGET  := $(shell uname -s | tr '[A-Z]' '[a-z]' 2>/dev/null || echo unknown)

SRC  := src/server.c src/net.c src/ae.c


ifeq ($(TARGET), sunos)
	CFLAGS += -D_PTHREADS -D_POSIX_C_SOURCE=200112L
	LIBS   += -lsocket
else ifeq ($(TARGET), darwin)
	# Per https://luajit.org/install.html: If MACOSX_DEPLOYMENT_TARGET
	# is not set then it's forced to 10.4, which breaks compile on Mojave.
	export MACOSX_DEPLOYMENT_TARGET = $(shell sw_vers -productVersion)
	LDFLAGS += -pagezero_size 10000 -image_base 100000000
	LIBS += -L/usr/local/opt/openssl/lib
	CFLAGS += -I/usr/local/include -I/usr/local/opt/openssl/include
	SRC += src/ae_kqueue.c
else ifeq ($(TARGET), linux)
        CFLAGS  += -D_POSIX_C_SOURCE=200809L -D_BSD_SOURCE
	LIBS    += -ldl
	LDFLAGS += -Wl,-E
	SRC += src/ae_epoll.c
else ifeq ($(TARGET), freebsd)
	CFLAGS  += -D_DECLARE_C99_LDBL_MATH
	LDFLAGS += -Wl,-E
	SRC += src/ae_select.c
endif

BIN  := oneProxy

ODIR := obj
OBJ  := $(patsubst %.c,$(ODIR)/%.o,$(SRC))

LDIR     = deps/luajit/src
LIBS    := -lluajit $(LIBS)
CFLAGS  += -I$(LDIR)
LDFLAGS += -L$(LDIR)

$(ODIR):
	@mkdir -p $@

$(ODIR)/%.o:  $(SRC)
	@echo $<
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

server: $(ODIR)/%.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) obj/*

.PHONY: server clean
