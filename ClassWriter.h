
#ifndef _CLASS_WRITER_H
#define _CLASS_WRITER_H

#include <stdint.h>

class ClassWriter
{
public:
  ClassWriter();
  ~ClassWriter();

  int add_field();
  int add_method();
  int write();

private:
  int constants_count;
  uint32_t *constants_index;
  uint8_t *constants;

  int methods_count;
  uint8_t *methods;

  int fields_count;
  uint8_t *fields;
};


#endif

