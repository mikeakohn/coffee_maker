#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClassWriter.h"

ClassWriter::ClassWriter() : need_code(false)
{
  Constant constant;

  super_class = "java/lang/Object";

  constants.push_back(constant);
}

ClassWriter::~ClassWriter()
{

}

int ClassWriter::add_field(std::string name, std::string type, uint16_t access_flags)
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

  field.access_flags = access_flags;
  field.name = get_constant_utf8(name);
  field.type = get_constant_utf8(type);
  fields.push_back(field);

  constants.push_back(constant);

  return 0;
}

int ClassWriter::add_method(std::string name, std::string type, uint16_t access_flags, int max_stack, int max_locals, uint8_t *code, int code_length)
{
  Constant constant;
  Method method;

  if (class_name == "")
  {
    return -1;
  }

  if (code_length != 0) { need_code = true; }

  constant.tag = 10;
  constant.class_index = get_constant_class(class_name);
  constant.name_and_type = get_constant_name_and_type(name, type);

  method.access_flags = access_flags;
  method.name = get_constant_utf8(name);
  method.type = get_constant_utf8(type);
  method.max_stack = max_stack;
  method.max_locals = max_locals;
  method.code = (uint8_t *)malloc(code_length);
  memcpy(method.code, code, code_length);
  method.code_length = code_length;
  methods.push_back(method);

  constants.push_back(constant);

  return 0;
}

int ClassWriter::get_field_id(std::string name)
{
  Constant constant;
  int index;

  for (index = 1; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 9)
    {
      int name_and_type = constants[index].name_and_type;

      if (constants[constants[name_and_type].name].text == name)
      {
        return index;
      }
    }
  }

  return -1;
}

int ClassWriter::get_method_id(std::string name)
{
  Constant constant;
  int index;

  for (index = 1; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 10)
    {
      int name_and_type = constants[index].name_and_type;

      if (constants[constants[name_and_type].name].text == name)
      {
        return index;
      }
    }
  }

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
  get_constant_class(super_class);

  // If we have methods that aren't native, need a "Code" string
  std::string code = "Code";
  if (need_code) { get_constant_class(code); }

  if (write_constants(buffer, len, ptr) < 0) { return -1; }

  // Write access flags
  if (len < ptr + 6) { return -1; }
  buffer[ptr++] = access_flags >> 8;
  buffer[ptr++] = access_flags & 0xff;

  index = get_constant_class(class_name);
  buffer[ptr++] = index >> 8;
  buffer[ptr++] = index & 0xff;
  index = get_constant_class(super_class);
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

  for (index = 1; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 7)
    {
      if (constants[constants[index].name].text == name)
      {
        return index;
      }
    }
  }

  constant.tag = 7;
  constant.name = get_constant_utf8(name);
  constants.push_back(constant);

  return constants.size() - 1;
}

int ClassWriter::get_constant_name_and_type(std::string &name, std::string &type)
{
  Constant constant;

  constant.tag = 12;
  constant.name = get_constant_utf8(name);
  constant.type = get_constant_utf8(type);
  constants.push_back(constant);

  return constants.size() - 1;
}

int ClassWriter::get_constant_utf8(std::string &text)
{
  Constant constant;
  int index;

  for (index = 1; index < (int)constants.size(); index++)
  {
    if (constants[index].tag == 1 && text == constants[index].text)
    {
      return index;
    }
  }

  constant.tag = 1;
  constant.text = text;
  constants.push_back(constant);

  return constants.size() - 1;
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
    if (iter->tag == 9 || iter->tag == 10)
    {
      // FIELD OR METHOD
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
  uint16_t fields_count = fields.size();
  std::vector<Field>::iterator iter;

  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = fields_count >> 8;
  buffer[ptr++] = fields_count & 0xff;

  for (iter = fields.begin(); iter < fields.end(); iter++)
  {
    if (len < ptr + 8) { return -1; }
    buffer[ptr++] = iter->access_flags >> 8;
    buffer[ptr++] = iter->access_flags & 0xff;
    buffer[ptr++] = iter->name >> 8;
    buffer[ptr++] = iter->name & 0xff;
    buffer[ptr++] = iter->type >> 8;
    buffer[ptr++] = iter->type & 0xff;
    buffer[ptr++] = 0;
    buffer[ptr++] = 0;
  }

  return 0;
}

int ClassWriter::write_methods(uint8_t *buffer, int len, int &ptr)
{
  uint16_t methods_count = methods.size();
  std::vector<Method>::iterator iter;

  if (len < ptr + 2) { return -1; }

  buffer[ptr++] = methods_count >> 8;
  buffer[ptr++] = methods_count & 0xff;

  for (iter = methods.begin(); iter < methods.end(); iter++)
  {
    int attribute_length = 2 + 2 + 4 + iter->code_length + 2 + 2;

    if (len < ptr + 8 + 24 + iter->code_length) { return -1; }
    buffer[ptr++] = iter->access_flags >> 8;
    buffer[ptr++] = iter->access_flags & 0xff;
    buffer[ptr++] = iter->name >> 8;
    buffer[ptr++] = iter->name & 0xff;
    buffer[ptr++] = iter->type >> 8;
    buffer[ptr++] = iter->type & 0xff;
    buffer[ptr++] = 0;
    buffer[ptr++] = 1;

    // Attribute name
    std::string code = "Code";
    uint16_t index = get_constant_utf8(code);
    buffer[ptr++] = index >> 8;
    buffer[ptr++] = index & 0xff;

    // Length of this attribute
    buffer[ptr++] = (attribute_length >> 24) & 0xff;
    buffer[ptr++] = (attribute_length >> 16) & 0xff;
    buffer[ptr++] = (attribute_length >> 8) & 0xff;
    buffer[ptr++] = attribute_length & 0xff;

    buffer[ptr++] = iter->max_stack >> 8;
    buffer[ptr++] = iter->max_stack & 0xff;

    buffer[ptr++] = iter->max_locals >> 8;
    buffer[ptr++] = iter->max_locals & 0xff;

    buffer[ptr++] = (iter->code_length >> 24) & 0xff;
    buffer[ptr++] = (iter->code_length >> 16) & 0xff;
    buffer[ptr++] = (iter->code_length >> 8) & 0xff;
    buffer[ptr++] = iter->code_length & 0xff;

    memcpy(buffer + ptr, iter->code, iter->code_length);
    ptr += iter->code_length;

    // Exception table length
    buffer[ptr++] = 0;
    buffer[ptr++] = 0;

    // Attributes count
    buffer[ptr++] = 0;
    buffer[ptr++] = 0;

    free(iter->code);
  }

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


