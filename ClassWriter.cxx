#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClassWriter.h"

ClassWriter::ClassWriter()
{
  super_class = "java/lang/Object";
}

ClassWriter::~ClassWriter()
{

}

int ClassWriter::add_field(std::string name, std::string type)
{
  Constant constant;
  Field field;

  if (class_name == "")
  {
    return -1;
  }

  constant.tag = 9;
  constant.class_index = get_constant_class(class_name);
  constant.name_and_type = get_constant_name_and_type(name, type);

  field.index = constants.size();
  fields.push_back(field);

  constants.push_back(constant);

  return 0;
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
  get_constant_class(class_name);
  get_constant_utf8(super_class);

  if (write_constants(buffer, len, ptr) < 0) { return -1; }

  // Write access flags
  if (len < ptr + 6) { return -1; }
  buffer[ptr++] = access_flags >> 8;
  buffer[ptr++] = access_flags & 0xff;

  index = get_constant_utf8(class_name);
  buffer[ptr++] = index >> 8;
  buffer[ptr++] = index & 0xff;
  index = get_constant_utf8(super_class);
  buffer[ptr++] = index >> 8;
  buffer[ptr++] = index & 0xff;

  if (write_interfaces(buffer, len, ptr) < 0) { return -1; }
  if (write_fields(buffer, len, ptr) < 0) { return -1; }
  if (write_methods(buffer, len, ptr) < 0) { return -1; }
  if (write_attributes(buffer, len, ptr) < 0) { return -1; }

  return ptr;
}

int ClassWriter::get_constant_class(std::string &name)
{
  Constant constant;
  int index;

  for (index = 0; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 7)
    {
      if (constants[constants[index].name].text == name)
      {
        return index + 1;
      }
    }
  }

  constant.tag = 7;
  constant.name = get_constant_utf8(name);
  constants.push_back(constant);

  return constants.size();
}

int ClassWriter::get_constant_name_and_type(std::string &name, std::string &type)
{
  Constant constant;

  constant.tag = 12;
  constant.name = get_constant_utf8(name);
  constant.type = get_constant_utf8(type);
  constants.push_back(constant);

  return constants.size();
}

int ClassWriter::get_constant_utf8(std::string &text)
{
  Constant constant;
  int index;

  for (index = 0; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 1 && text == constants[index].text)
    {
      return index + 1;
    }
  }

  constant.tag = 1;
  constant.text = text;
  constants.push_back(constant);

  return constants.size();
}

int ClassWriter::write_constants(uint8_t *buffer, int len, int &ptr)
{
  uint16_t constants_count = constants.size() + 1;
  std::vector<Constant>::iterator iter;

  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = constants_count >> 8;
  buffer[ptr++] = constants_count & 0xff;

  for (iter = constants.begin(); iter < constants.end(); iter++)
  {
    if (iter->tag == 1)
    {
      // UTF8
      uint16_t text_length = (uint16_t)iter->text.size();

      if (len < ptr + 3 + text_length) { return -1; }
      buffer[ptr++] = iter->tag;
      buffer[ptr++] = text_length >> 8;
      buffer[ptr++] = text_length & 0xff;
      memcpy(buffer + ptr, iter->text.c_str(), text_length);
      ptr += text_length;
    }
      else
    if (iter->tag == 7)
    {
      // CLASS
      if (len < ptr + 3) { return -1; }
      buffer[ptr++] = iter->tag;
      buffer[ptr++] = iter->name >> 8;
      buffer[ptr++] = iter->name & 0xff;
    }
      else
    if (iter->tag == 9)
    {
      // FIELD
      if (len < ptr + 5) { return -1; }
      buffer[ptr++] = iter->tag;
      buffer[ptr++] = iter->class_index >> 8;
      buffer[ptr++] = iter->class_index & 0xff;
      buffer[ptr++] = iter->name_and_type >> 8;
      buffer[ptr++] = iter->name_and_type & 0xff;
    }
      else
    if (iter->tag == 12)
    {
      // NAME AND TYPE
      if (len < ptr + 5) { return -1; }
      buffer[ptr++] = iter->tag;
      buffer[ptr++] = iter->name >> 8;
      buffer[ptr++] = iter->name & 0xff;
      buffer[ptr++] = iter->type >> 8;
      buffer[ptr++] = iter->type & 0xff;
    }
  }

  return ptr;
}

int ClassWriter::write_interfaces(uint8_t *buffer, int len, int &ptr)
{
  uint16_t interfaces_count = 0;
  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = interfaces_count >> 8;
  buffer[ptr++] = interfaces_count & 0xff;

  return 0;
}

int ClassWriter::write_fields(uint8_t *buffer, int len, int &ptr)
{
  uint16_t fields_count = fields.size() + 1;
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

int ClassWriter::write_attributes(uint8_t *buffer, int len, int &ptr)
{
  uint16_t attributes_count = 0;
  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = attributes_count >> 8;
  buffer[ptr++] = attributes_count & 0xff;

  return 0;
}


