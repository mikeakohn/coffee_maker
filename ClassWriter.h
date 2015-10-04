
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

#define FIELD_ACCESS_PUBLIC 0x0001
#define FIELD_ACCESS_PRIVATE 0x0002
#define FIELD_ACCESS_PROTECTED 0x0004
#define FIELD_ACCESS_STATIC 0x0008
#define FIELD_ACCESS_FINAL 0x0010
#define FIELD_ACCESS_VOLATILE 0x0040
#define FIELD_ACCESS_TRANSIENT 0x0080
#define FIELD_ACCESS_SYNTHETIC 0x1000
#define FIELD_ACCESS_ENUM 0x4000

#define METHOD_ACCESS_PUBLIC 0x0001
#define METHOD_ACCESS_PRIVATE 0x0002
#define METHOD_ACCESS_PROTECTED 0x0004
#define METHOD_ACCESS_STATIC 0x0008
#define METHOD_ACCESS_FINAL 0x0010
#define METHOD_ACCESS_SYNCHRONIZED 0x0020
#define METHOD_ACCESS_BRIDGE 0x0040
#define METHOD_ACCESS_VARARGS 0x0080
#define METHOD_ACCESS_NATIVE 0x0100
#define METHOD_ACCESS_ABSTRACT 0x0400
#define METHOD_ACCESS_STRICT 0x0800
#define METHOD_ACCESS_SYNTHETIC 0x1000

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
  Field() : access_flags(0), name(0), type(0) { }
  int access_flags;
  int name;
  int type;
};

class Method
{
public:
  Method() : access_flags(0), name(0), type(0), max_stack(0), max_locals(0), code(NULL), code_length(0) { }
  int access_flags;
  int name;
  int type;
  int max_stack;
  int max_locals;
  uint8_t *code;
  int code_length;
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
  int add_field(std::string name, std::string type, uint16_t access_flags);
  int add_method(std::string name, std::string type, uint16_t access_flags, int max_stack, int max_locals, uint8_t *code, int code_length);
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

  bool need_code;

  std::vector<Constant> constants;
  std::vector<Field> fields;
  std::vector<Method> methods;
};


#endif

