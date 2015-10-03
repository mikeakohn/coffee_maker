
#ifndef _CLASS_WRITER_H
#define _CLASS_WRITER_H

#include <stdint.h>
#include <string>
#include <vector>

#define JAVA_VERSION_5 49
#define JAVA_VERSION_6 50
#define JAVA_VERSION_7 51
#define JAVA_VERSION_8 52

#define CLASS_ACCESS_PUBLIC 0x0001
#define CLASS_ACCESS_FINAL 0x0010
#define CLASS_ACCESS_SUPER 0x0020
#define CLASS_ACCESS_INTERFACE 0x0200
#define CLASS_ACCESS_ABSTRACT 0x0400
#define CLASS_ACCESS_SYNTHETIC 0x1000
#define CLASS_ACCESS_ANNOTATION 0x2000
#define CLASS_ACCESS_ENUM 0x4000

class Constant
{
public:
  Constant() : tag(0), class_index(0), name_and_type(0), name(0), type(0) { }
  uint8_t tag;
  int class_index;
  int name_and_type;
  int name;
  int type;
  std::string text;
};

class Field
{
public:
  Field() : index(0), class_index(0), name(0), type(0) { }
  int index;
  int class_index;
  int name;
  int type;
};

class ClassWriter
{
public:
  ClassWriter();
  ~ClassWriter();

  void set_class_name(const char *value) { class_name = value; }
  void set_super_class(const char *value) { super_class = value; }
  void set_major_version(uint16_t value) { major_version = value; }
  void set_minor_version(uint16_t value) { minor_version = value; }
  void set_access_flags(uint16_t value) { access_flags = value; }
  int add_field(std::string name, std::string type);
  int add_method();
  int write(uint8_t *buffer, int len);

private:
  int get_constant_class(std::string &name);
  int get_constant_name_and_type(std::string &name, std::string &type);
  int get_constant_utf8(std::string &text);
  int write_constants(uint8_t *buffer, int len, int &ptr);
  int write_interfaces(uint8_t *buffer, int len, int &ptr);
  int write_fields(uint8_t *buffer, int len, int &ptr);
  int write_methods(uint8_t *buffer, int len, int &ptr);
  int write_attributes(uint8_t *buffer, int len, int &ptr);

  std::string class_name;
  std::string super_class;
  uint16_t minor_version;
  uint16_t major_version;
  uint16_t access_flags;

  std::vector<Constant> constants;
  std::vector<Field> fields;

  int methods_count;
  uint8_t *methods;
};


#endif

