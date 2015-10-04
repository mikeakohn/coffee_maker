#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ClassWriter.h"

int main(int argc, char *argv[])
{
  FILE *out;
  ClassWriter *class_writer;
  uint8_t buffer[65536];
  uint8_t code[4096];
  int len;

  printf("\ncoffee_maker by Michael Kohn\n"
         "http://www.mikekohn.net/\n\n");

  if (argc != 2)
  {
    printf("Usage: %s <filename.class>\n", argv[0]);
    exit(0);
  }

  out = fopen(argv[1], "wb");

  if (out == NULL)
  {
    printf("Could not open %s for writing\n", argv[1]);
    exit(1);
  }

  class_writer = new ClassWriter();

  class_writer->set_major_version(JAVA_VERSION_7);
  class_writer->set_class_name("TestClass");
  class_writer->set_access_flags(CLASS_ACCESS_PUBLIC);
  class_writer->add_field("field1", "I", FIELD_ACCESS_PUBLIC);
  class_writer->add_field("field2", "I", FIELD_ACCESS_PUBLIC);

  code[0] = 0x1a;
  code[1] = 0x1b;
  code[2] = 0x60;
  code[3] = 0xac;

  class_writer->add_method("addNums", "(II)I", METHOD_ACCESS_PUBLIC | METHOD_ACCESS_STATIC, 2, 2, code, 4);

#if 0
  printf("field_id=%d\n", class_writer->get_field_id("field1"));
  printf("field_id=%d\n", class_writer->get_field_id("field2"));
  printf("method_id=%d\n", class_writer->get_method_id("addNums"));
#endif

  len = class_writer->write(buffer, sizeof(buffer));
  delete class_writer;

  if (len > 0)
  {
    fwrite(buffer, len, 1, out);
  }
    else
  {
    printf("Error creating class\n");
  }

  fclose(out);

  return 0;
}

