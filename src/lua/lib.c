//#define LUA_LIB
#include <lua.h>
#include <lauxlib.h>

#include <client.h>

#include <string.h>

#define MAX_BUFFER_LEN 4096

static int unix_sockets_close(lua_State *L)
{
    int sock = lua_getfield(L, 1, "fd");
    socket_close(sock);

    return 0;
}

static int unix_sockets_read(lua_State *L)
{
    char buff[MAX_BUFFER_LEN];
    int sock = lua_getfield(L, 1, "fd");
    int res;

    res = socket_read(sock, buff, MAX_BUFFER_LEN);
    if (res <= 0) {
        lua_pushnil(L);
        return 1;
    }

    buff[res-1] = '\0';
    lua_pushstring(L, buff);

    return 1;
}

static int unix_sockets_write(lua_State *L)
{
    int res;
    int sock = lua_getfield(L, 1, "fd");
    const char *buffer = lua_tostring(L, 2);

    res = socket_write(sock, buffer, strlen(buffer));
    if (res <= 0)
        lua_pushnil(L);
    else
        lua_pushboolean(L, 1);

    return 1;
}

static const luaL_Reg socket_methods[] = {
    {"close", unix_sockets_close },
    {"read",  unix_sockets_read },
    {"write", unix_sockets_write },
    {NULL, NULL}
};

static int unix_sockets_open(lua_State *L)
{
    const char *name = lua_tostring(L, 1);
    int sock = socket_open(name);
    if (sock < 0) {
        lua_pushnil(L);
        return 1;
    }

    luaL_newlib(L, socket_methods);
    lua_pushinteger(L, sock);
    lua_setfield(L, 2, "fd");

    return 1;
}

static const luaL_Reg unix_sockets_lib[] = {
    {"open",  unix_sockets_open },
    {NULL, NULL}
};

int luaopen_libunixsockets(lua_State *L)
{
    luaL_newlib(L, unix_sockets_lib);

    return 1;
}
