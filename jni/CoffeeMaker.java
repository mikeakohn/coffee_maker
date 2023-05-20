/**
 *  Coffee Maker
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: BSD
 *
 * Copyright 2015-2023 by Michael Kohn
 *
 * This is the Java part of the JNI code for CoffeeMaker.cxx. This has
 * code for generating class files in memory that can be loaded into the
 * JVM at runtime.
*/

import java.lang.ClassLoader;
// REMOVE
import java.io.FileOutputStream;
import java.io.BufferedWriter;
import java.io.FileWriter;

public class CoffeeMaker extends ClassLoader
{
  public CoffeeMaker()
  {
    handle = openClassWriter();
  }

  native public static void start();
  native public static void stop();
  native private long openClassWriter();
  native public void setClassName(String value);
  native public void setSuperClass(String value);
  native public void setMajorVersion(int value);
  native public void setAccessFlags(int value);
  native public int addField(String name, String type, short access_flags);
  native public int addField(String name, String type, short access_flags, boolean is_inherited);
  native public int addMethod(String name, String type, short access_flags, int max_stack, int max_locals, byte[] code);
  native public int addMethodExternal(String name, String type, String class_name);
  native public int getFieldId(String name);
  native public int getMethodId(String name);
  native public int getFieldId(String name, String type, String class_name);
  native public int getMethodId(String name, String type, String class_name);
  native public byte[] create();

  public Class<?> load()
  {
    byte[] code = create();
    Class<?> cls;

/*
try
{
FileOutputStream writer = new FileOutputStream("TestClass.class");
writer.write(code);
writer.close();
}
catch (Exception e)
{
  System.out.println(e.toString());
}
*/

    cls = defineClass(null, code, 0, code.length);
    resolveClass(cls);

    return cls;
  }

  static
  {
    System.loadLibrary("coffee_maker");
  }

  private long handle;

  public static final short JAVA_VERSION_5 = 49;
  public static final short JAVA_VERSION_6 = 50;
  public static final short JAVA_VERSION_7 = 51;
  public static final short JAVA_VERSION_8 = 52;

  public static final short CLASS_ACCESS_PUBLIC = 0x0001;
  public static final short CLASS_ACCESS_FINAL = 0x0010;
  public static final short CLASS_ACCESS_SUPER = 0x0020;
  public static final short CLASS_ACCESS_INTERFACE = 0x0200;
  public static final short CLASS_ACCESS_ABSTRACT = 0x0400;
  public static final short CLASS_ACCESS_SYNTHETIC = 0x1000;
  public static final short CLASS_ACCESS_ANNOTATION = 0x2000;
  public static final short CLASS_ACCESS_ENUM = 0x4000;

  public static final short FIELD_ACCESS_PUBLIC = 0x0001;
  public static final short FIELD_ACCESS_PRIVATE = 0x0002;
  public static final short FIELD_ACCESS_PROTECTED = 0x0004;
  public static final short FIELD_ACCESS_STATIC = 0x0008;
  public static final short FIELD_ACCESS_FINAL = 0x0010;
  public static final short FIELD_ACCESS_VOLATILE = 0x0040;
  public static final short FIELD_ACCESS_TRANSIENT = 0x0080;
  public static final short FIELD_ACCESS_SYNTHETIC = 0x1000;
  public static final short FIELD_ACCESS_ENUM = 0x4000;

  public static final short METHOD_ACCESS_PUBLIC = 0x0001;
  public static final short METHOD_ACCESS_PRIVATE = 0x0002;
  public static final short METHOD_ACCESS_PROTECTED = 0x0004;
  public static final short METHOD_ACCESS_STATIC = 0x0008;
  public static final short METHOD_ACCESS_FINAL = 0x0010;
  public static final short METHOD_ACCESS_SYNCHRONIZED = 0x0020;
  public static final short METHOD_ACCESS_BRIDGE = 0x0040;
  public static final short METHOD_ACCESS_VARARGS = 0x0080;
  public static final short METHOD_ACCESS_NATIVE = 0x0100;
  public static final short METHOD_ACCESS_ABSTRACT = 0x0400;
  public static final short METHOD_ACCESS_STRICT = 0x0800;
  public static final short METHOD_ACCESS_SYNTHETIC = 0x1000;
}

