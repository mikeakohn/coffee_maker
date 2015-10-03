
#ifndef _CLASS_WRITER_H
#define _CLASS_WRITER_H

struct _class_writer
{
  int constants_count;
  uint32_t *constants_index;
  uint8_t *constants;

  int methods_count;
  uint8_t *methods;

  int fields_count;
  uint8_t *fields;
};

struct _class_writer *class_writer_alloc();
int class_writer_add_field(struct _class_writer *class_writer);
int class_writer_add_method(struct _class_writer *class_writer);
int class_writer_write(struct _class_writer *class_writer);
int class_writer_free(struct _class_writer *class_writer);

#endif

