#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <jni.h>

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

  jfieldID field_var0;
  jfieldID field_var1;
  jfieldID field_var2;
  jfieldID field_var3;
  jfieldID field_var4;
  jfieldID field_var5;
  jfieldID field_var6;
  jfieldID field_var7;
  jfieldID field_var8;
  jfieldID field_var9;
  jfieldID field_var10;
  jfieldID field_var11;
  jfieldID field_var12;
  jfieldID field_var13;
  jfieldID field_var14;
  jfieldID field_var15;
  jfieldID field_var16;
  jfieldID field_var17;
  jfieldID field_var18;
  jfieldID field_var19;

JNIEXPORT void JNICALL Java_MyObject_start(JNIEnv *env, jobject obj)
{
  asm __volatile__
  (
    "rdtsc" : "=a" (perf_start.split.lo), "=d" (perf_start.split.hi)
  );
}

JNIEXPORT void JNICALL Java_MyObject_stop(JNIEnv *env, jobject obj)
{
  asm __volatile__
  (
    "rdtsc" : "=a" (perf_stop.split.lo), "=d" (perf_stop.split.hi)
  );

  printf("cpu=%ld\n", perf_stop.count - perf_start.count);
}

JNIEXPORT void JNICALL Java_MyObject_init(JNIEnv *env, jobject obj)
{
  jclass cls = (*env)->GetObjectClass(env, obj);
field_var0 = (*env)->GetFieldID(env, cls, "field0", "I");
field_var1 = (*env)->GetFieldID(env, cls, "field1", "I");
field_var2 = (*env)->GetFieldID(env, cls, "field2", "I");
field_var3 = (*env)->GetFieldID(env, cls, "field3", "I");
field_var4 = (*env)->GetFieldID(env, cls, "field4", "I");
field_var5 = (*env)->GetFieldID(env, cls, "field5", "I");
field_var6 = (*env)->GetFieldID(env, cls, "field6", "I");
field_var7 = (*env)->GetFieldID(env, cls, "field7", "I");
field_var8 = (*env)->GetFieldID(env, cls, "field8", "I");
field_var9 = (*env)->GetFieldID(env, cls, "field9", "I");
field_var10 = (*env)->GetFieldID(env, cls, "field10", "I");
field_var11 = (*env)->GetFieldID(env, cls, "field11", "I");
field_var12 = (*env)->GetFieldID(env, cls, "field12", "I");
field_var13 = (*env)->GetFieldID(env, cls, "field13", "I");
field_var14 = (*env)->GetFieldID(env, cls, "field14", "I");
field_var15 = (*env)->GetFieldID(env, cls, "field15", "I");
field_var16 = (*env)->GetFieldID(env, cls, "field16", "I");
field_var17 = (*env)->GetFieldID(env, cls, "field17", "I");
field_var18 = (*env)->GetFieldID(env, cls, "field18", "I");
field_var19 = (*env)->GetFieldID(env, cls, "field19", "I");
}

JNIEXPORT void JNICALL Java_MyObject_jniLoad(
  JNIEnv *env,
  jobject obj,
  jbyteArray array)
{
  int ptr = (*env)->GetArrayLength(env, array) - 4;
  int num;

  //uint8_t *data = (uint8_t *)(*env)->GetByteArrayElements(env, array, NULL);
  uint8_t *data = (uint8_t *)(*env)->GetPrimitiveArrayCritical(env, array, 0);

  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var0, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var1, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var2, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var3, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var4, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var5, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var6, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var7, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var8, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var9, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var10, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var11, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var12, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var13, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var14, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var15, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var16, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var17, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var18, num);
  ptr -= 4;
  num = *((int *)(&data[ptr]));
  (*env)->SetIntField(env, obj, field_var19, num);
  ptr -= 4;

  //(*env)->ReleaseByteArrayElements(env, array, (int8_t *)data, 0);
  (*env)->ReleasePrimitiveArrayCritical(env, array, data, 0);
}


