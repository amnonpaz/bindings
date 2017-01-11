#include <Python.h>
#include <client.h>

#define MAX_BUFFER_LEN 4096

static PyObject *unixsockets_open(PyObject *self, PyObject *args)
{
    const char *socket_name;
    int fd;

    if (!PyArg_ParseTuple(args, "s", &socket_name))
        return NULL;

    fd = socket_open(socket_name);
    if (fd <= 0) {
        PyErr_SetFromErrno(PyExc_Exception);
        return NULL;
    }

    return Py_BuildValue("i", fd);
}


static PyObject *unixsockets_close(PyObject *self, PyObject *args)
{
    int fd;

    if (!PyArg_ParseTuple(args, "i", &fd))
        return NULL;

    socket_close(fd);

    return Py_BuildValue("i", 0);
}

static PyObject *unixsockets_write(PyObject *self, PyObject *args)
{
    const char *str;
    int fd, res, len;

    if (!PyArg_ParseTuple(args, "is#", &fd, &str, &len))
        return NULL;

    res = socket_write(fd, str, len);
    if (res <= 0) {
        PyErr_SetFromErrno(PyExc_Exception);
        return NULL;
    }

    return Py_BuildValue("i", res);
}

static PyObject *unixsockets_read(PyObject *self, PyObject *args)
{
    char str[MAX_BUFFER_LEN];
    int fd, res;

    if (!PyArg_ParseTuple(args, "i", &fd))
        return NULL;

    res = socket_read(fd, str, MAX_BUFFER_LEN);
    if (res <= 0) {
        PyErr_SetFromErrno(PyExc_Exception);
        return NULL;
    }
    str[res] = '\0';

    return Py_BuildValue("s", str);
}

static PyMethodDef unixSocketsMethods[] = {
    {"open", unixsockets_open, METH_VARARGS, "Open a Unix domain socket"},
    {"close", unixsockets_close, METH_VARARGS, "Close a Unix domain socket"},
    {"write", unixsockets_write, METH_VARARGS, "Write to a Unix domain socket"},
    {"read", unixsockets_read, METH_VARARGS, "Write to a Unix domain socket"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initUnixSockets(void)
{
    (void)Py_InitModule("UnixSockets", unixSocketsMethods);
}
