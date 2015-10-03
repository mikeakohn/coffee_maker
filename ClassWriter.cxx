#include <stdio.h>
#include <stdlib.h>

struct _class_writer *class_writer_alloc()
{
  struct _class_writer *class_writer;

  class_writer = (struct _class_writer *)malloc(sizeof(struct _class_writer));
  memset(class_writer, 0, sizeof(struct _class_writer));

  return class_writer;
}

int class_writer_add_field(struct _class_writer *class_writer)
{
  return -1;
}

int class_writer_add_method(struct _class_writer *class_writer)
{
  return -1;
}

int class_writer_write(struct _class_writer *class_writer)
{
  return -1;
}

int class_writer_free(struct _class_writer *class_writer)
{
  return -1;
}

