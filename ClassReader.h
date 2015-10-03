
#ifndef _CLASS_READER_H
#define _CLASS_READER_H

#include <stdint.h>

class ClassReader
{
public:
  static int dump_class(uint8_t *buffer);

private:
  static int dump_header(uint8_t *buffer);
  static int dump_constants(uint8_t *buffer, int ptr, struct _constants *constants);
  static void show_name(uint8_t *buffer, struct _constants *constants, int index);
  static void show_class(uint8_t *buffer, struct _constants *constants, int index);
  static int dump_info(uint8_t *buffer, int ptr, struct _constants *constants);
  static int dump_interfaces(uint8_t *buffer, int ptr, struct _constants *constants);
  static int dump_attributes(uint8_t *buffer, int ptr, struct _constants *constants);
  static int dump_fields(uint8_t *buffer, int ptr, struct _constants *constants);
  static int dump_methods(uint8_t *buffer, int ptr, struct _constants *constants);
};

#endif

