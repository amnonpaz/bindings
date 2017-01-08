CC = gcc
CFLAGS = -g -Wall -fPIC
RM = rm -f
LD = ld
LDFLAGS = -fPIC -share

CLIENT_INC = -Iunix_sockets/src/include/
CLIENT_LIB = unix_sockets/libclient.a

TARGET_DIR = build
LIBNAME = libunixsockets.so

## Lua definitions
LUAV ?= $(shell lua -v | awk '{ split($$2, t, ".") ; if (t[1] >= 5) { v = t[1] "." t[2] } ; print v }')

LIBLUA = liblua
LIBLUA_OBJS = src/lua/lib.o
LIBLUA_LIBS = $(shell pkg-config --libs-only-l lua$(LUAV))
LIBLUA_CFLAGS = $(shell pkg-config --cflags lua$(LUAV))
LIBLUA_TARGET_SUBDIR = lua
LIBLUA_TARGET = $(TARGET_DIR)/$(LIBLUA_TARGET_SUBDIR)/$(LIBNAME)
LIBLUA_DEST_DIR ?= /usr/local/lib/lua/$(LUAV)/

## Java definitions
LIBJAVA = libjava
LIBJAVA_OBJS = src/java/unix_sockets_jni.o
LIBJAVA_CFLAGS = -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
LIBJAVA_TARGET_SUBDIR = java
LIBJAVA_TARGET = $(TARGET_DIR)/$(LIBJAVA_TARGET_SUBDIR)/$(LIBNAME)
LIBJAVA_DEST_DIR ?= /usr/lib/jni/

## All targets
all: clean $(LIBLUA) $(LIBJAVA)

## Adding CFLAGS according to target
$(LIBLUA): CFLAGS += $(LIBLUA_CFLAGS)
$(LIBJAVA): CFLAGS += $(LIBJAVA_CFLAGS)

## Recipes
$(LIBLUA): client_lib $(LIBLUA_OBJS)
	@echo "Linking $@..."
	@mkdir -p $(TARGET_DIR)/$(LIBLUA_TARGET_SUBDIR)
	@$(LD) $(LDFLAGS) $(LIBLUA_LIBS) -o $(LIBLUA_TARGET) $(LIBLUA_OBJS) $(CLIENT_LIB)

$(LIBJAVA): client_lib $(LIBJAVA_OBJS)
	@echo "Linking $@..."
	@mkdir -p $(TARGET_DIR)/$(LIBJAVA_TARGET_SUBDIR)
	@$(LD) $(LDFLAGS) -o $(LIBJAVA_TARGET) $(LIBJAVA_OBJS) $(CLIENT_LIB)

%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(CLIENT_INC) -c $< -o $@

client_lib:
	@echo "Building unix sockets client library..."
	@make -C unix_sockets clientlib

install:
	@echo "Installing $(LIBLUA_TARGET) in $(LIBLUA_DEST_DIR)..."
	@mkdir -p $(LIBLUA_DEST_DIR)
	@install -m 655 $(LIBLUA_TARGET) $(LIBLUA_DEST_DIR)/$(LIBNAME)
	@echo "Installing $(LIBJAVA_TARGET) in $(LIBJAVA_DEST_DIR)..."
	@mkdir -p $(LIBJAVA_DEST_DIR)
	@install -m 655 $(LIBJAVA_TARGET) $(LIBJAVA_DEST_DIR)/$(LIBNAME)

uninstall:
	@echo "Deleting $(LIBLUA_DEST_DIR)/$(LIBLUA_TARGET)"
	@rm -f $(LIBLUA_DEST_DIR)/$(LIBNAME)
	@echo "Deleting $(LIBJAVA_DEST_DIR)/$(LIBJAVA_TARGET)"
	@rm -f $(LIBLUA_JAVA_DIR)/$(LIBNAME)

clean:
	@echo "Cleaning unix_sockets..."
	@make -C unix_sockets clean
	@echo "Cleaning projects..."
	@$(RM) $(LIBLUA_TARGET) $(LIBLUA_OBJS) $(LIBJAVA_TARGET) $(LIBJAVA_OBJS)

.PHONY: all clean client_lib install uninstall $(LIBLUA) $(LIBJAVA)
