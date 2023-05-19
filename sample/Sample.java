/**
 *  Coffee Maker
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: BSD
 *
 * Copyright 2015-2023 by Michael Kohn
 *
 * This is a test program that generates a class file in memory
 * using CoffeeMaker. The code sets a bunch of fields in the TestData
 * class by loading up some hand-written Java byte code and loads it
 * into a class as a byte[] and then loaded in the Java JVM.
 *
 * This also tests setting the field using JNI to compare the speed
 * between a Java generated method and straight JNI.
 *
*/

import java.lang.reflect.Method;

public class Sample
{
  public static byte[] generateReverseNums(CoffeeMaker coffee_maker)
  {
    byte[] code = new byte[641];
    int field_num = 0;
    int ptr = 0;
    int n;

    for (n = 80; n > 0; n = n - 4)
    {
      code[ptr++] = 0x2a; // aload_0
      code[ptr++] = 0x2b; // aload_1
      code[ptr++] = 0x10; // bipush n - 4
      code[ptr++] = (byte)(n - 4);
      code[ptr++] = 0x33; // baload
      code[ptr++] = 0x2b; // aload_1
      code[ptr++] = 0x10; // bipush n - 3
      code[ptr++] = (byte)(n - 3);
      code[ptr++] = 0x33; // baload
      code[ptr++] = 0x10; // bipush 8
      code[ptr++] = 8;
      code[ptr++] = 0x78; // ishl
      code[ptr++] = (byte)0x80; // ior
      code[ptr++] = 0x2b; // aload_1
      code[ptr++] = 0x10; // bipush n - 2
      code[ptr++] = (byte)(n - 2);
      code[ptr++] = 0x33; // baload
      code[ptr++] = 0x10; // bipush 16
      code[ptr++] = 16;
      code[ptr++] = 0x78; // ishl
      code[ptr++] = (byte)0x80; // ior
      code[ptr++] = 0x2b; // aload_1
      code[ptr++] = 0x10; // bipush n - 1
      code[ptr++] = (byte)(n - 1);
      code[ptr++] = 0x33; // baload
      code[ptr++] = 0x10; // bipush 24
      code[ptr++] = 24;
      code[ptr++] = 0x78; // ishl
      code[ptr++] = (byte)0x80; // ior

      int index = coffee_maker.getFieldId("field" + field_num, "I", "TestData");
      code[ptr++] =  -75;  // putfield
      code[ptr++] = (byte)((index >> 8) & 0xff);
      code[ptr++] = (byte)(index & 0xff);

      field_num++;
    }

    code[ptr++] =  -79;  // return

    return code;
  }

  public static void main(String[] args)
  {
    TestData test_data = new TestData();
    CoffeeMaker coffee_maker = new CoffeeMaker();
    int i;
    byte[] data;
    byte[] add_nums = { 0x1a, 0x1b, 0x60, -84 };
    byte[] reverse_nums;

    reverse_nums = generateReverseNums(coffee_maker);

    coffee_maker.setMajorVersion(CoffeeMaker.JAVA_VERSION_7);
    coffee_maker.setClassName("TestClass");
    coffee_maker.setAccessFlags(CoffeeMaker.CLASS_ACCESS_PUBLIC);
    coffee_maker.addField("field1", "I", CoffeeMaker.FIELD_ACCESS_PUBLIC);
    coffee_maker.addMethod("addNums", "(II)I",
       (short)(CoffeeMaker.METHOD_ACCESS_PUBLIC |
               CoffeeMaker.METHOD_ACCESS_STATIC),
       2, 2, add_nums);
    coffee_maker.addMethod("reverseNums", "(LTestData;[B)V",
       (short)(CoffeeMaker.METHOD_ACCESS_PUBLIC |
               CoffeeMaker.METHOD_ACCESS_STATIC),
       4, 2, reverse_nums);

    //System.out.println(coffee_maker.getFieldId("field1"));
    //System.out.println(coffee_maker.getMethodId("addNums"));

    Class<?> cls = coffee_maker.load();
    Class add_nums_args[] = { int.class, int.class };
    Class reverse_nums_args[] = { TestData.class, byte[].class };
    Method addNums = null;
    Method reverseNums = null;

    try
    {
      addNums = cls.getDeclaredMethod("addNums", add_nums_args);
      reverseNums = cls.getDeclaredMethod("reverseNums", reverse_nums_args);

      System.out.println(addNums.invoke(null, 5, 3));
    }
    catch (Exception e)
    {
      System.out.println(e.toString());
    }

    data = new byte[args.length * 4];
    test_data.initByteBuffer(data);

    System.out.println("--- Testing JNI ---");

    for (i = 0; i < 10; i++)
    {
      TestData.timerStart();
      test_data.jniLoad(data);
      TestData.timerStop();
    }

    System.out.println("--- Testing Java (from source javaLoad(byte[])) ---");

    for (i = 0; i < 10; i++)
    {
      TestData.timerStart();
      test_data.javaLoad(data);
      TestData.timerStop();
    }

    System.out.println("--- Testing Java (from source javaLoadByteBuffer(byte[])) ---");

    for (i = 0; i < 10; i++)
    {
      TestData.timerStart();
      test_data.javaLoadByteBuffer(data);
      TestData.timerStop();
    }

    System.out.println("--- Testing Java (from generated) ---");

    for (i = 0; i < 10; i++)
    {
      TestData.timerStart();
      try
      {
        reverseNums.invoke(null, test_data, data);
      }
      catch (Exception e)
      {
        System.out.println(e.toString());
        System.out.println(e.getCause().toString());
      }
      TestData.timerStop();
    }

    System.out.println(test_data.field0);
  }
}

