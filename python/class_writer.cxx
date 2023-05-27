#include <stdio.h>
#include <stdlib.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ClassWriter.h"

typedef struct
{
  PyObject_HEAD
  ClassWriter *class_writer;
  // FIXME: This should be an std::string or something.
  char class_name[128];
} PyClassWriterObject;

static void class_writer_dealloc(PyClassWriterObject *object)
{
  delete object->class_writer;
  object->class_writer = nullptr;
  Py_DECREF(object);
  PyObject_Del(object);
}

PyTypeObject class_writer_type =
{
  PyObject_HEAD_INIT(0)
  //0,
  "ClassWriter",
  sizeof(PyClassWriterObject),
  0,
  (destructor)class_writer_dealloc,
};

static PyObject *create(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object =
    PyObject_NEW(PyClassWriterObject, &class_writer_type);

  //PyObject_Init((PyObject *)object, &class_writer_type);

  //Py_INCREF(object);
  //Py_NewRef(object);

  object->class_writer = new ClassWriter();
  object->class_writer->set_class_name("Unnamed");
  sprintf(object->class_name, "Unnamed.class");

  return (PyObject *)object;
}

static PyObject *write(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  uint8_t data[65536];

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  int length = object->class_writer->write(data, sizeof(data));

  FILE *fp = fopen(object->class_name, "wb");
  fwrite(data, 1, length, fp);
  fclose(fp);

  //delete object->class_writer;
  //object->class_writer = nullptr;

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *set_class_name(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *value;

  if (!PyArg_ParseTuple(args, "Os", &object, &value))
  {
    return NULL;
  }

  snprintf(object->class_name, sizeof(object->class_name), "%s.class", value);
  object->class_writer->set_class_name(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *set_super_class(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *value;

  if (!PyArg_ParseTuple(args, "Os", &object, &value))
  {
    return NULL;
  }

  object->class_writer->set_super_class(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *set_major_version(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->class_writer->set_major_version(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *set_minor_version(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->class_writer->set_minor_version(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *set_access_flags(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->class_writer->set_access_flags(value);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *add_field(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *name = nullptr;
  char *type = nullptr;
  int access_flags = 0;
  int is_inherited = 0;

  if (!PyArg_ParseTuple(args, "Ossii", &object, &name, &type, &access_flags, &is_inherited))
  {
    return NULL;
  }

  object->class_writer->add_field(name, type, access_flags, is_inherited);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *add_method(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *name = nullptr;
  char *type = nullptr;
  int access_flags = 0;
  int max_stack = 0;
  int max_locals = 0;
  uint8_t *code = nullptr;
  Py_ssize_t code_length;

  if (!PyArg_ParseTuple(args, "Ossiiis#", &object, &name, &type, &access_flags, &max_stack, &max_locals, &code, &code_length))
  {
    return NULL;
  }

  object->class_writer->add_method(
    name,
    type,
    access_flags,
    max_stack,
    max_locals,
    code,
    code_length);

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *get_method_id(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *name = nullptr;
  char *type = nullptr;
  char *class_name = nullptr;
  int id;

  if (!PyArg_ParseTuple(args, "Os", &object, &name))
  {
    id = object->class_writer->get_method_id(name);
    return Py_BuildValue("i", id);;
  }

  if (PyArg_ParseTuple(args, "Osss", &object, &name, &type, &class_name))
  {
    id = object->class_writer->get_method_id(name, type, class_name);
    return Py_BuildValue("i", id);;
  }

  return NULL;
}

static PyObject *get_field_id(
  PyObject *self,
  PyObject *args)
{
  PyClassWriterObject *object;
  char *name = nullptr;
  char *type = nullptr;
  char *class_name = nullptr;
  int id;

  if (PyArg_ParseTuple(args, "Os", &object, &name))
  {
    id = object->class_writer->get_field_id(name);
    return Py_BuildValue("i", id);;
  }

  if (PyArg_ParseTuple(args, "Osss", &object, &name, &type, &class_name))
  {
    id = object->class_writer->get_field_id(name, type, class_name);
    return Py_BuildValue("i", id);;
  }

  return NULL;
}

static PyMethodDef ClassWriterMethods[] =
{
  { "create", create, METH_VARARGS, "Start a new Java class file." },
  { "write", write, METH_VARARGS, "Write the class file to disk." },
  { "set_class_name", set_class_name, METH_VARARGS, "Set the class name." },
  { "set_super_class", set_super_class, METH_VARARGS, "Set the super class." },
  { "set_major_version", set_major_version, METH_VARARGS, "Set the compiler major version." },
  { "set_minor_version", set_minor_version, METH_VARARGS, "Set the compiler minor version." },
  { "set_access_flags", set_access_flags, METH_VARARGS, "Set the class access flags." },
  { "add_field", add_field, METH_VARARGS, "Add a field to the class." },
  { "add_method", add_method, METH_VARARGS, "Add a method to the class." },
  { "get_method_id", get_method_id, METH_VARARGS, "Get the id from the constant pool of a method." },
  { "get_field_id", get_field_id, METH_VARARGS, "Get the id from the constant pool of a field." },
  { NULL, NULL, 0, NULL }
};

static struct PyModuleDef class_writer_module =
{
  PyModuleDef_HEAD_INIT,
  "class_writer",
  "Python interface for ClassWriter.",
  -1,
  ClassWriterMethods
};

PyMODINIT_FUNC PyInit_class_writer(void)
{
  return PyModule_Create(&class_writer_module);
}

