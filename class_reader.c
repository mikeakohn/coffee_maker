/**
 *  Coffee Maker
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: BSD
 *
 * Copyright 2015-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "class_reader.h"

#define GET_INT16(a, ptr) \
  ((a[ptr] << 8) | \
  (a[ptr+1]))

#define GET_INT32(a, ptr) \
  ((a[ptr] << 24) | \
   (a[ptr+1] << 16) | \
   (a[ptr+2] << 8) | \
   (a[ptr+3]))

#define GET_INT64(a, ptr) \
    (int64_t)\
 ((((uint64_t)a[ptr+0]) << 56) | \
  (((uint64_t)a[ptr+1]) << 48) | \
  (((uint64_t)a[ptr+2]) << 40) | \
  (((uint64_t)a[ptr+3]) << 32) | \
  (((uint64_t)a[ptr+4]) << 24) | \
  (((uint64_t)a[ptr+5]) << 16) | \
  (((uint64_t)a[ptr+6]) << 8) | \
   ((uint64_t)a[ptr+7]))

struct _constants
{
  int count;
  uint32_t *index;
};

static int dump_header(uint8_t *buffer)
{
  const char *version;

  switch (GET_INT16(buffer, 6))
  {
    case 45: version = "1.1"; break;
    case 46: version = "1.2"; break;
    case 47: version = "1.3"; break;
    case 48: version = "1.4"; break;
    case 49: version = "5.0"; break;
    case 50: version = "6.0"; break;
    case 51: version = "7"; break;
    case 52: version = "8"; break;
    case 53: version = "9"; break;
    case 54: version = "10"; break;
    case 55: version = "11"; break;
    case 56: version = "12"; break;
    case 57: version = "13"; break;
    case 58: version = "14"; break;
    case 59: version = "15"; break;
    case 60: version = "16"; break;
    case 61: version = "17"; break;
    case 62: version = "18"; break;
    default: version = "???"; break;
  }

  printf("--------------------------------------------------------\n");
  printf("  Magic Number: %02X%02X%02X%02X\n",
         buffer[0], buffer[1], buffer[2], buffer[3]);
  printf(" Minor Version: %d\n", GET_INT16(buffer, 4));
  printf(" Major Version: %d (%s)\n", GET_INT16(buffer, 6), version);

  return 8;
}

static int dump_constants(
  uint8_t *buffer,
  int ptr,
  struct _constants *constants)
{
  uint16_t constant_count = GET_INT16(buffer, ptr);
  int index;
  int count;
  uint8_t tag;
  float f;
  double d;
  long l;
  int i;
  void *temp;

  ptr += 2;

  printf("--------------------------------------------------------\n");
  printf("Constant Count: %d\n", constant_count);

  constants->count = constant_count;
  constants->index = (uint32_t *)calloc(constant_count, 4);

  for (index = 0; index < constant_count - 1; index++)
  {
    constants->index[index + 1] = ptr;
    printf("%d) ", index + 1);
    tag = buffer[ptr];

    switch (tag)
    {
      case 1:
        count = GET_INT16(buffer, ptr + 1);
        printf("tag=%d UTF-8: ", tag);
        for (i = 0; i < count; i++) { printf("%c", buffer[ptr + 3 + i]); }
        printf(" (%d)\n", count);
        ptr += count + 3;
        break;
      case 3:
        printf("tag=%d int: %d\n", tag, GET_INT32(buffer, ptr + 1));
        ptr += 5;
        break;
      case 4:
        i = GET_INT32(buffer, ptr + 1);
        temp = (void *)&i;
        f = *(float *)temp;
        printf("tag=%d float: %f\n", tag, f);
        ptr += 5;
        break;
      case 5:
        printf("tag=%d long: 0x%lx\n", tag, GET_INT64(buffer, ptr + 1));
        ptr += 9;
        index++;
        break;
      case 6:
        l = GET_INT64(buffer, ptr + 1);
        temp = (void*)&l;
        d = *(double *)temp;
        printf("tag=%d double: %f\n", tag, d);
        ptr += 9;
        index++;
        break;
      case 7:
        printf("tag=%d class: %d\n", tag, GET_INT16(buffer, ptr + 1));
        ptr += 3;
        break;
      case 8:
        printf("tag=%d string: %d\n", tag, GET_INT16(buffer, ptr + 1));
        ptr += 3;
        break;
      case 9:
        printf("tag=%d field: class=%d name_type=%d\n",
               tag,
               GET_INT16(buffer, ptr + 1),
               GET_INT16(buffer, ptr + 3));
        ptr += 5;
        break;
      case 10:
        printf("tag=%d method: class=%d name_type=%d\n",
               tag,
               GET_INT16(buffer, ptr + 1),
               GET_INT16(buffer, ptr + 3));
        ptr += 5;
        break;
      case 11:
        printf("tag=%d interface: class=%d name_type=%d\n",
               tag,
               GET_INT16(buffer, ptr + 1),
               GET_INT16(buffer, ptr + 3));
        ptr += 5;
        break;
      case 12:
        printf("tag=%d name_type: name=%d type=%d\n",
               tag,
               GET_INT16(buffer, ptr + 1),
               GET_INT16(buffer, ptr + 3));
        ptr += 5;
        break;
      case 15:
        printf("tag=%d method_handle: type=%d constant=%d\n",
               tag,
               buffer[ptr + 1],
               GET_INT16(buffer, ptr + 2));
        ptr += 4;
        break;
      case 16:
        printf("tag=%d method_type: type=%d\n",
               tag,
               GET_INT16(buffer, ptr + 1));
        ptr += 3;
        break;
      case 18:
        printf("tag=%d invoke_dynamic: data=%d\n",
               tag,
               GET_INT32(buffer, ptr + 1));
        ptr += 5;
        break;
      default:
        printf("Unknown constant tag %d at %d\n", tag, ptr);
        break;
    }
  }

  return ptr;
}

static void show_name(uint8_t *buffer, struct _constants *constants, int index)
{
  uint16_t count;
  uint8_t tag;
  int ptr,i;

  ptr = constants->index[index];
  tag = buffer[ptr];

  if (tag != 1) { return; }

  count = GET_INT16(buffer, ptr + 1);

  for (i = 0; i < count; i++)
  {
    printf("%c", buffer[ptr + 3 + i]);
  }
}

static void show_class(uint8_t *buffer, struct _constants *constants, int index)
{
  uint8_t tag;
  int ptr;

  if (index > constants->count) { return; }

  ptr = constants->index[index];
  tag = buffer[ptr];

  if (tag != 7) { return; }

  show_name(buffer, constants, GET_INT16(buffer, ptr + 1));
}

static int dump_info(uint8_t *buffer, int ptr, struct _constants *constants)
{
  int access_flags = GET_INT16(buffer, ptr);
  int this_class_index = GET_INT16(buffer, ptr + 2);
  int super_class_index = GET_INT16(buffer, ptr+ 4);

  printf("  Access Flags: (%d)", access_flags);
  if ((access_flags & 0x0001) != 0) { printf(" public"); }
  if ((access_flags & 0x0010) != 0) { printf(" final"); }
  if ((access_flags & 0x0020) != 0) { printf(" super"); }
  if ((access_flags & 0x0200) != 0) { printf(" interface"); }
  if ((access_flags & 0x0400) != 0) { printf(" abstract"); }
  if ((access_flags & 0x1000) != 0) { printf(" synthetic"); }
  if ((access_flags & 0x2000) != 0) { printf(" annotation"); }
  if ((access_flags & 0x4000) != 0) { printf(" enum"); }
  printf("\n");
  
  printf("    This Class: (%d) ", this_class_index);
  show_class(buffer, constants, this_class_index);
  printf("\n");
  printf("   Super Class: (%d) ", super_class_index);
  show_class(buffer, constants, super_class_index);
  printf("\n");

  return ptr + 6;
}

static int dump_interfaces(
  uint8_t *buffer,
  int ptr,
  struct _constants *constants)
{
  int interface_count = GET_INT16(buffer, ptr);
  uint16_t name_index;
  int i;

  ptr += 2;

  printf("--------------------------------------------------------\n");
  printf("Interface Count: %d\n", interface_count);

  for (i = 0; i < interface_count; i++)
  {
    name_index = GET_INT16(buffer, ptr + 1);

    printf("%d) name_index=%d ", i, name_index);
    show_class(buffer, constants, name_index);
    printf("\n");

    ptr += 2;
  }

  return ptr;
}

static int dump_attributes(
  uint8_t *buffer,
  int ptr,
  struct _constants *constants)
{
  uint16_t attribute_name_index;
  uint32_t attribute_length;
  uint32_t k;

  attribute_name_index = GET_INT16(buffer, ptr);
  attribute_length = GET_INT32(buffer, ptr + 2);

  printf("    ");
  show_name(buffer, constants, attribute_name_index);
  printf(" (%d) length=%d\n", attribute_name_index, attribute_length);

  ptr += 6;
  for (k = 0; k < attribute_length; k++)
  {
    if ((k & 7) == 0)
    {
      if (k != 0) { printf("\n"); }
      printf("    ");
    }
    printf(" %02x", buffer[ptr++]);
  }

  printf("\n");

  return ptr;
}

static int dump_fields(uint8_t *buffer, int ptr, struct _constants *constants)
{
  int field_count = GET_INT16(buffer, ptr);
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  int i, j;

  ptr += 2;

  printf("--------------------------------------------------------\n");
  printf("Field Count: %d\n", field_count);

  for (i = 0; i < field_count; i++)
  {
    access_flags = GET_INT16(buffer, ptr);
    name_index = GET_INT16(buffer, ptr + 2);
    descriptor_index = GET_INT16(buffer, ptr + 4);
    attributes_count = GET_INT16(buffer, ptr + 6);

    printf("%3d) name_index=%d descriptor_index=%d attributes_count=%d\n",
           i, name_index, descriptor_index, attributes_count);
    printf("    ");
    if ((access_flags & 0x0001) != 0) { printf("public "); }
    if ((access_flags & 0x0002) != 0) { printf("private "); }
    if ((access_flags & 0x0004) != 0) { printf("protected "); }
    if ((access_flags & 0x0008) != 0) { printf("static "); }
    if ((access_flags & 0x0010) != 0) { printf("final "); }
    if ((access_flags & 0x0040) != 0) { printf("volatile "); }
    if ((access_flags & 0x0080) != 0) { printf("transient "); }
    if ((access_flags & 0x1000) != 0) { printf("synthetic "); }
    if ((access_flags & 0x4000) != 0) { printf("enum "); }
    printf("(");
    show_name(buffer, constants, descriptor_index);
    printf(") ");
    show_name(buffer, constants, name_index);
    printf("\n");

    ptr = ptr + 8;

    for (j = 0; j < attributes_count; j++)
    {
      ptr = dump_attributes(buffer, ptr, constants);
    }
  }

  return ptr;
}

static int dump_methods(uint8_t *buffer, int ptr, struct _constants *constants)
{
  int method_count = GET_INT16(buffer, ptr);
  uint16_t access_flags;
  uint16_t name_index;
  uint16_t descriptor_index;
  uint16_t attributes_count;
  int i, j;

  ptr += 2;

  printf("--------------------------------------------------------\n");
  printf("Methods Count: %d\n", method_count);

  for (i = 0; i < method_count; i++)
  {
    access_flags = GET_INT16(buffer, ptr);
    name_index = GET_INT16(buffer, ptr + 2);
    descriptor_index = GET_INT16(buffer, ptr + 4);
    attributes_count = GET_INT16(buffer, ptr + 6);

    printf("%3d) name_index=%d descriptor_index=%d attributes_count=%d\n",
           i, name_index, descriptor_index, attributes_count);
    printf("    ");
    if ((access_flags & 0x0001) != 0) { printf("public "); }
    if ((access_flags & 0x0002) != 0) { printf("private "); }
    if ((access_flags & 0x0004) != 0) { printf("protected "); }
    if ((access_flags & 0x0008) != 0) { printf("static "); }
    if ((access_flags & 0x0010) != 0) { printf("final "); }
    if ((access_flags & 0x0020) != 0) { printf("synchronized "); }
    if ((access_flags & 0x0040) != 0) { printf("bridge "); }
    if ((access_flags & 0x0080) != 0) { printf("varargs "); }
    if ((access_flags & 0x0100) != 0) { printf("native "); }
    if ((access_flags & 0x0400) != 0) { printf("abstract "); }
    if ((access_flags & 0x0800) != 0) { printf("strict "); }
    if ((access_flags & 0x1000) != 0) { printf("synthetic "); }
    printf("(");
    show_name(buffer, constants, descriptor_index);
    printf(") ");
    show_name(buffer, constants, name_index);
    printf("\n");

    ptr = ptr + 8;

    for (j = 0; j < attributes_count; j++)
    {
      ptr = dump_attributes(buffer, ptr, constants);
    }
  }

  return ptr;
}

int dump_class(uint8_t *buffer)
{
  int ptr, i;
  int attributes_count;
  struct _constants constants;

  memset(&constants, 0, sizeof(constants));

  ptr = dump_header(buffer);
  ptr = dump_constants(buffer, ptr, &constants);
  ptr = dump_info(buffer, ptr, &constants);
  ptr = dump_interfaces(buffer, ptr, &constants);
  ptr = dump_fields(buffer, ptr, &constants);
  ptr = dump_methods(buffer, ptr, &constants);

  attributes_count = GET_INT16(buffer, ptr);
  ptr += 2;

  printf("--------------------------------------------------------\n");
  printf("Attributes Count: %d\n", attributes_count);


  for (i = 0; i < attributes_count; i++)
  {
    ptr = dump_attributes(buffer, ptr, &constants);
  }

  free(constants.index);

  return 0;
}

