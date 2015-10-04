#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <jni.h>

#include "ClassWriter.h"

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
JNIEXPORT long JNICALL Java_CoffeeMaker_openClassWriter(
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

// native public byte[] create();
JNIEXPORT jbyteArray JNICALL Java_CoffeeMaker_create(
  JNIEnv *env,
  jobject obj,
  jstring name_s,
  jstring type_s,
  jshort access_flags)
{
  uint8_t buffer[65536];
  ClassWriter *class_writer;
  class_writer = (ClassWriter *)env->GetLongField(obj, handle);
  int len = class_writer->write(buffer, sizeof(buffer));

for (int i = 0; i < len; i++)
{
  printf(" %02x", buffer[i]);
}
printf("\n");
  if (len < 0) { return NULL; }

  return NULL;
}

