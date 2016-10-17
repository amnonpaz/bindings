.PHONY: all clean download

UNIX_SOCKETS_GIT_URL = https://github.com/amnonpaz/unix_sockets.git

CC := gcc
CFLAGS := -g -Wall -fPIC
RM := rm -f
LD := ld
LDFLAGS := -fPIC -share 

CLIENT_INC := -Idownloads/unix_sockets/src/include/
CLIENT_LIB := downloads/unix_sockets/libclient.a

LUAV ?= $(shell lua -v | awk '{ split($$2, t, ".") ; if (t[1] >= 5) { v = t[1] "." t[2] } ; print "lua"v }')

LUA_LIBS := $(shell pkg-config --libs-only-l $(LUAV))
LUA_CFLAGS := $(shell pkg-config --cflags $(LUAV))

CFLAGS += $(LUA_CFLAGS)

## User targets
LIBLUA = liblua

## Actual targets
LIBLUA_TARGET = unix_sockets.so
LIBLUA_OBJS = src/lua/lib.o

## All targets
all: clean $(LIBLUA_TARGET)

## User targets to actual targets
$(LIBLUA): $(LIBLUA_TARGET)

## Recipes
$(LIBLUA_TARGET): download $(LIBLUA_OBJS)
	@echo "Linking $@"
	@$(LD) $(LDFLAGS) $(LUA_LIBS) -o $@ $(LIBLUA_OBJS) $(CLIENT_LIB)

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(CLIENT_INC) -c $< -o $@

download:
	@echo "Retreiving unix_sockets..."
	@mkdir -p downloads 
	@[ -e downloads/unix_sockets ] && git -C downloads/unix_sockets pull || git -C downloads clone $(UNIX_SOCKETS_GIT_URL)
	@make -C downloads/unix_sockets clientlib

clean:
	@echo "Cleaning unix_sockets..."
	@[ -e downloads/unix_sockets ] && make -C downloads/unix_sockets clean
	@echo "Cleaning projects..."
	@$(RM) $(TARGETS) $(LIBLUA_OBJS)
