//#include "Python.h"
//#include "structmember.h"
//
//typedef struct {
//    PyObject_HEAD
//    unsigned long FieldID;
//} CBashUINT32;
//
//static void
//CBashUINT32_dealloc(CBashUINT32* self)
//    {
//    self->ob_type->tp_free((PyObject*)self);
//    }
//
//static PyObject *
//CBashUINT32_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
//    {
//    CBashUINT32 *self;
//
//    self = (CBashUINT32 *)type->tp_alloc(type, 0);
//    if(self != NULL)
//        {
//        //initialize
//        self->FieldID = 0;
//        }
//
//    return (PyObject *)self;
//    }
//
//static int
//CBashUINT32_init(CBashUINT32 *self, PyObject *args, PyObject *kwds)
//    {
//    PyObject *fieldID=NULL, *tmp;
//
//    static char *kwlist[] = {"FieldID", NULL};
//
//    if(!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, &self->FieldID))
//        return -1; 
//
//    return 0;
//    }
//
//static PyObject *
//CBashUINT32_get(CBashUINT32* self)
//    {
//
//    result = PyString_Format(format, args);
//
//    return PyLong_FromUnsignedLong(self->FieldID);
//    }
//
//
//static PyMethodDef CBashUINT32_methods[] = {
//    {"__get__", (PyCFunction)Noddy_name, METH_NOARGS,
//     "Return the name, combining the first and last name"
//    },
//    {NULL}  /* Sentinel */
//};
//
//
//
//PyMODINIT_FUNC
//init_cinttest(void)