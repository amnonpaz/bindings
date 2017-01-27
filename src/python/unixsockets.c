#include <Python.h>
#include <client.h>

#define MAX_BUFFER_LEN 4096

typedef struct {
    PyObject_HEAD
    int fd;
} socket;

static void socketDealloc(socket* self)
{
    if (self->fd > 0)
        socket_close(self->fd);

    self->ob_type->tp_free((PyObject*)self);
}

static PyObject *socketNew(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    socket *self;

    self = (socket *)type->tp_alloc(type, 0);
    if (self == NULL)
        return NULL;

    self->fd = 0;

    return (PyObject *)self;
}

static int socketInit(socket *self, PyObject *args, PyObject *kwds)
{
    const char *socket_name;

    if (self->fd != 0) {
        PyErr_SetString(PyExc_BaseException,
                        "Cannot initialize an open socket");
        return -1;
    }

    if (!PyArg_ParseTuple(args, "s", &socket_name))
        return -1;

    self->fd = socket_open(socket_name);
    if (self->fd <= 0) {
        PyErr_SetFromErrno(PyExc_IOError);
        return -1;
    }

    return 0;
}

static PyObject *socketWrite(socket *self, PyObject *args)
{
    const char *str;
    int res, len;

    if (!PyArg_ParseTuple(args, "s#", &str, &len))
        return NULL;

    res = socket_write(self->fd, str, len);
    if (res <= 0) {
        PyErr_SetFromErrno(PyExc_IOError);
        return NULL;
    }

    return Py_BuildValue("i", res);
}

static PyObject *socketRead(socket *self)
{
    char str[MAX_BUFFER_LEN];
    int res;

    res = socket_read(self->fd, str, MAX_BUFFER_LEN);
    if (res <= 0) {
        PyErr_SetFromErrno(PyExc_IOError);
        return NULL;
    }
    str[res] = '\0';

    return Py_BuildValue("s", str);
}

static PyMethodDef unixSocketsMethods[] = {
    { "write", (PyCFunction)socketWrite, METH_VARARGS, "Write to a Unix domain socket" },
    { "read", (PyCFunction)socketRead, METH_NOARGS, "Read from a Unix domain socket" },
    { NULL, NULL, 0, NULL }
};

static PyTypeObject socketType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "UnixSockets.Socket",      /*tp_name*/
    sizeof(socket),            /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)socketDealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Unix socket",             /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    unixSocketsMethods,        /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)socketInit,      /* tp_init */
    0,                         /* tp_alloc */
    socketNew,                 /* tp_new */
};

static PyMethodDef unixSocketsModuleMethods[] = {
    { NULL }
};

PyMODINIT_FUNC initUnixSockets(void)
{
    PyObject* m;

    if (PyType_Ready(&socketType) < 0)
        return;

    m = Py_InitModule3("UnixSockets", unixSocketsModuleMethods,
                       "Unix socket modules");
    if (m == NULL)
        return;

    Py_INCREF(&socketType);
    PyModule_AddObject(m, "Socket", (PyObject *)&socketType);
}
