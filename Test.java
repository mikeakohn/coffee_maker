
import java.lang.reflect.Method;

public class Test
{
  public static void main(String[] args)
  {
    CoffeeMaker coffee_maker = new CoffeeMaker();
    byte[] code = { 0x1a, 0x1b, 0x60, -84 };

    code[0] = 0x1a;
    code[1] = 0x1b;
    code[2] = 0x60;
    code[3] = (byte)(int)0xac;

    coffee_maker.setMajorVersion(CoffeeMaker.JAVA_VERSION_7);
    coffee_maker.setClassName("TestClass");
    coffee_maker.setAccessFlags(CoffeeMaker.CLASS_ACCESS_PUBLIC);
    coffee_maker.addField("field1", "I", CoffeeMaker.FIELD_ACCESS_PUBLIC);
    coffee_maker.addMethod("addNums", "(II)I",
       (short)(CoffeeMaker.METHOD_ACCESS_PUBLIC |
               CoffeeMaker.METHOD_ACCESS_STATIC),
       2, 2, code);

    //System.out.println(coffee_maker.getFieldId("field1"));
    //System.out.println(coffee_maker.getMethodId("addNums"));

    Class<?> cls = coffee_maker.load();
    Class method_args[] = { int.class, int.class };
/*
    Class method_args[] = new Class[2];
    method_args[0] = int.class;
    method_args[1] = int.class;
*/

    try
    {
      Method addNums = cls.getDeclaredMethod("addNums", method_args);
      System.out.println(addNums.invoke(null, 5, 3));
    }
    catch (Exception e)
    {
      System.out.println(e.toString());
    }
  }
}

