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
  ClassWriter *class_writer = new ClassWriter();

  return (jlong)class_writer;
}

// native public void setClassName(String value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setClassName(
  JNIEnv *env,
  jobject obj,
  jstring value)
{
}

// native public void setSuperClass(String value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setSuperClass(
  JNIEnv *env,
  jobject obj,
  jstring value)
{
}

// native public void setMajorVersion(int value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setMajorVersion(
  JNIEnv *env,
  jobject obj,
  jshort value)
{
}

// native public void setAccessFlags(int value);
JNIEXPORT void JNICALL Java_CoffeeMaker_setAccessFlags(
  JNIEnv *env,
  jobject obj,
  jshort value)
{
}

// native public int addField(String name, String type, short access_flags);
JNIEXPORT int JNICALL Java_CoffeeMaker_addField(
  JNIEnv *env,
  jobject obj,
  jstring name,
  jstring type,
  jshort access_flags)
{
  return 0;
}




