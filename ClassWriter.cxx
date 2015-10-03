#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClassWriter.h"

ClassWriter::ClassWriter() :
  //constants_count(0),
  methods_count(0),
  fields_count(0)
{
  super_class = "java/lang/Object";
}

ClassWriter::~ClassWriter()
{

}

int ClassWriter::add_field()
{
  return -1;
}

int ClassWriter::add_method()
{
  return -1;
}

int ClassWriter::write(uint8_t *buffer, int len)
{
  uint16_t index;
  int ptr = 8;

  // Check if len of this buffer is less than at least 26 bytes
  if (len < 26) { return -1; }

  // Set magic number
  buffer[0] = 0xca;
  buffer[1] = 0xfe;
  buffer[2] = 0xba;
  buffer[3] = 0xbe;

  // Set version info
  buffer[4] = minor_version >> 8;
  buffer[5] = minor_version & 0xff;
  buffer[6] = major_version >> 8;
  buffer[7] = major_version & 0xff;

  // Get constant index for this class and super class or insert it
  get_constant(class_name);
  get_constant(super_class);

  if (write_constants(buffer, len, ptr) < 0) { return -1; }

printf("len=%d ptr=%d\n", len, ptr);
  // Write access flags
  if (len < ptr + 6) { return -1; }
  buffer[ptr++] = access_flags >> 8;
  buffer[ptr++] = access_flags & 0xff;

  index = get_constant(class_name);
  buffer[ptr++] = index >> 8;
  buffer[ptr++] = index & 0xff;
  index = get_constant(super_class);
  buffer[ptr++] = index >> 8;
  buffer[ptr++] = index & 0xff;

  if (write_fields(buffer, len, ptr) < 0) { return -1; }
  if (write_methods(buffer, len, ptr) < 0) { return -1; }

  return ptr;
}

int ClassWriter::get_constant(std::string &name)
{
  Constant constant;
  int index;

  for (index = 0; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 1 && name == constants[index].text)
    {
      return index;
    }
  }

  constant.tag = 1;
  constant.text = name;
  constants.push_back(constant);

  return index;
}

int ClassWriter::write_constants(uint8_t *buffer, int len, int &ptr)
{
  uint16_t constants_count = constants.size();
  std::vector<Constant>::iterator iter;

  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = constants_count >> 8;
  buffer[ptr++] = constants_count & 0xff;

  for (iter = constants.begin(); iter < constants.end(); iter++)
  {
    if (iter->tag == 1)
    {
      uint16_t text_length = (uint16_t)iter->text.size();

      if (len < ptr + 3 + text_length) { return -1; }
      buffer[ptr++] = iter->tag;
      buffer[ptr++] = text_length >> 8;
      buffer[ptr++] = text_length & 0xff;
      memcpy(buffer + ptr, iter->text.c_str(), text_length);
      ptr += text_length;
//printf("constant: %s %d\n", iter->text.c_str(), (int)iter->text.size());
    }
  }

  return ptr;
}

int ClassWriter::write_fields(uint8_t *buffer, int len, int &ptr)
{
  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = fields_count >> 8;
  buffer[ptr++] = fields_count & 0xff;

  return 0;
}

int ClassWriter::write_methods(uint8_t *buffer, int len, int &ptr)
{
  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = methods_count >> 8;
  buffer[ptr++] = methods_count & 0xff;

  return 0;
}



