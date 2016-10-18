.PHONY: all clean download install uninstall

UNIX_SOCKETS_GIT_URL = https://github.com/amnonpaz/unix_sockets.git

CC := gcc
CFLAGS := -g -Wall -fPIC
RM := rm -f
LD := ld
LDFLAGS := -fPIC -share

CLIENT_INC := -Idownloads/unix_sockets/src/include/
CLIENT_LIB := downloads/unix_sockets/libclient.a

TARGET_DIR := targets

V ?= $(shell lua -v | awk '{ split($$2, t, ".") ; if (t[1] >= 5) { v = t[1] "." t[2] } ; print v }')
LUAV := lua$(V)

LUA_LIBS := $(shell pkg-config --libs-only-l $(LUAV))
LUA_CFLAGS := $(shell pkg-config --cflags $(LUAV))
LUA_DEST_DIR ?= /usr/local/lib/lua/$(V)/

CFLAGS += $(LUA_CFLAGS)

## User targets
LIBLUA := liblua
LIBLUA_TARGET_DIR := $(TARGET_DIR)/lua

## Actual targets
LIBLUA_TARGET := unix_sockets.so
LIBLUA_OBJS := src/lua/lib.o

## All targets
all: clean $(LIBLUA_TARGET)

## User targets to actual targets
$(LIBLUA): $(LIBLUA_TARGET)

## Recipes
$(LIBLUA_TARGET): download $(LIBLUA_OBJS)
	@echo "Linking $@"
	@mkdir -p $(LIBLUA_TARGET_DIR)
	@$(LD) $(LDFLAGS) $(LUA_LIBS) -o $(LIBLUA_TARGET_DIR)/$@ $(LIBLUA_OBJS) $(CLIENT_LIB)

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(CLIENT_INC) -c $< -o $@

download:
	@echo "Retreiving unix_sockets..."
	@mkdir -p downloads
	@[ -e downloads/unix_sockets ] && git -C downloads/unix_sockets pull || git -C downloads clone $(UNIX_SOCKETS_GIT_URL)
	@make -C downloads/unix_sockets clientlib

install:
	@echo "Installing $(LIBLUA_TARGET) in $(LUA_DEST_DIR)"
	@mkdir -p $(LUA_DEST_DIR)
	@install -m 655 $(LIBLUA_TARGET_DIR)/$(LIBLUA_TARGET) $(LUA_DEST_DIR)/$(LIBLUA_TARGET)

uninstall:
	@echo "Deleting $(LUA_DEST_DIR)/$(LIBLUA_TARGET)"
	@rm -f $(LUA_DEST_DIR)/$(LIBLUA_TARGET)

clean:
	@echo "Cleaning unix_sockets..."
	@[ -e downloads/unix_sockets ] && make -C downloads/unix_sockets clean
	@echo "Cleaning projects..."
	@$(RM) $(TARGETS) $(LIBLUA_OBJS)
