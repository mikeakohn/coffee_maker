/**
 *  Coffee Maker
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: BSD
 *
 * Copyright 2015-2023 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <jni.h>

#include "ClassWriter.h"

#ifdef __cplusplus
extern "C" {
#endif

union _perftime
{
  struct _split
  {
    uint32_t lo;
    uint32_t hi;
  } split;
  uint64_t count;
};

union _perftime perf_start;
union _perftime perf_stop;

// FIXME: Yes, this is awkward
jfieldID handle;

JNIEXPORT void JNICALL Java_CoffeeMaker_startTimer(JNIEnv *env, jobject obj)
{
  asm __volatile__
  (
    "rdtsc" : "=a" (perf_start.split.lo), "=d" (perf_start.split.hi)
  );
}

JNIEXPORT void JNICALL Java_CoffeeMaker_stopTimer(JNIEnv *env, jobject obj)
{
  asm __volatile__
  (
    "rdtsc" : "=a" (perf_stop.split.lo), "=d" (perf_stop.split.hi)
  );

  printf("cpu=%ld\n", perf_stop.count - perf_start.count);
}

// native public long openClassWriter();
JNIEXPORT jlong JNICALL Java_CoffeeMaker_openClassWriter(
  JNIEnv *env,
  jobject obj)
{
  jclass cls = env->GetObjectClass(obj);
  handle = env->GetFieldID(cls, "handle", "J");

  ClassWriter *class_writer = new ClassWriter();

  return (jlong)class_writer;
}

// native public void setClassName(String value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setClassName(
  JNIEnv *env,
  jobject obj,
  jstring value)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *class_name = env->GetStringUTFChars(value, 0);
  class_writer->set_class_name(class_name);
  env->ReleaseStringUTFChars(value, class_name);
}

// native public void setSuperClass(String value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setSuperClass(
  JNIEnv *env,
  jobject obj,
  jstring value)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *super_class = env->GetStringUTFChars(value, 0);
  class_writer->set_super_class(super_class);
  env->ReleaseStringUTFChars(value, super_class);
}

// native public void setMajorVersion(int value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setMajorVersion(
  JNIEnv *env,
  jobject obj,
  jshort value)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  class_writer->set_major_version(value);
}

// native public void setAccessFlags(int value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setAccessFlags(
  JNIEnv *env,
  jobject obj,
  jshort value)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  class_writer->set_access_flags(value);
}

// native public int addField(String name, String type, short access_flags);
JNIEXPORT int JNICALL Java_CoffeeMaker_addField(
  JNIEnv *env,
  jobject obj,
  jstring name_s,
  jstring type_s,
  jshort access_flags)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  const char *type = env->GetStringUTFChars(type_s, 0);

  class_writer->add_field(name, type, access_flags);

  env->ReleaseStringUTFChars(name_s, name);
  env->ReleaseStringUTFChars(type_s, type);

  return 0;
}

// native public int addMethod(String name, String type, short access_flags, int max_stack, int max_locals, byte[] code);
JNIEXPORT int JNICALL Java_CoffeeMaker_addMethod(
  JNIEnv *env,
  jobject obj,
  jstring name_s,
  jstring type_s,
  jshort access_flags,
  jint max_stack,
  jint max_locals,
  jbyteArray code_bytes)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  const char *type = env->GetStringUTFChars(type_s, 0);
  uint8_t *code = (uint8_t *)env->GetByteArrayElements(code_bytes, NULL);
  int code_length = env->GetArrayLength(code_bytes); 

  class_writer->add_method(name, type, access_flags, max_stack, max_locals, code, code_length);

  env->ReleaseStringUTFChars(name_s, name);
  env->ReleaseStringUTFChars(type_s, type);
  env->ReleaseByteArrayElements(code_bytes, (int8_t *)code, 0);

  return 0;
}

// native public int getFieldId(String name);
JNIEXPORT jint JNICALL Java_CoffeeMaker_getFieldId__Ljava_lang_String_2(
  JNIEnv *env,
  jobject obj,
  jstring name_s)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  int value = class_writer->get_field_id(name);
  env->ReleaseStringUTFChars(name_s, name);

  return value;
}

// native public int getMethodId(String name);
JNIEXPORT jint JNICALL Java_CoffeeMaker_getMethodId__Ljava_lang_String_2(
  JNIEnv *env,
  jobject obj,
  jstring name_s)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  int value = class_writer->get_method_id(name);
  env->ReleaseStringUTFChars(name_s, name);

  return value;
}

// native public int getFieldId(String name, String type, String class_name);
JNIEXPORT jint JNICALL Java_CoffeeMaker_getFieldId__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2(
  JNIEnv *env,
  jobject obj, 
  jstring name_s,
  jstring type_s,
  jstring class_name_s)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  const char *type = env->GetStringUTFChars(type_s, 0);
  const char *class_name = env->GetStringUTFChars(class_name_s, 0);

  int value = class_writer->get_field_id(name, type, class_name);

  env->ReleaseStringUTFChars(name_s, name);
  env->ReleaseStringUTFChars(type_s, type);
  env->ReleaseStringUTFChars(class_name_s, class_name);

  return value;
}

// native public int getMethodId(String name, String type, String class_name);
JNIEXPORT jint JNICALL Java_CoffeeMaker_getMethodId__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2(
  JNIEnv *env,
  jobject obj,
  jstring name_s,
  jstring type_s,
  jstring class_name_s)
{
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);

  const char *name = env->GetStringUTFChars(name_s, 0);
  const char *type = env->GetStringUTFChars(type_s, 0);
  const char *class_name = env->GetStringUTFChars(class_name_s, 0);

  int value = class_writer->get_method_id(name, type, class_name);

  env->ReleaseStringUTFChars(name_s, name);
  env->ReleaseStringUTFChars(type_s, type);
  env->ReleaseStringUTFChars(class_name_s, class_name);

  return value;
}

// native public byte[] create();
JNIEXPORT jbyteArray JNICALL Java_CoffeeMaker_create(
  JNIEnv *env,
  jobject obj,
  jstring name_s,
  jstring type_s,
  jshort access_flags)
{
  jbyteArray class_file;
  uint8_t buffer[65536];
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);
  int len = class_writer->write(buffer, sizeof(buffer));

#if 0
for (int i = 0; i < len; i++)
{
  printf(" %02x", buffer[i]);
}
printf("\n");
#endif

  if (len < 0) { return NULL; }

  class_file = env->NewByteArray(len);

  env->SetByteArrayRegion(class_file, 0, len, (jbyte *)buffer);

  return class_file;
}

#ifdef __cplusplus
}
#endif

