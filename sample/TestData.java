import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class TestData
{
  ByteBuffer byte_buffer;

  public int field0;
  public int field1;
  public int field2;
  public int field3;
  public int field4;
  public int field5;
  public int field6;
  public int field7;
  public int field8;
  public int field9;
  public int field10;
  public int field11;
  public int field12;
  public int field13;
  public int field14;
  public int field15;
  public int field16;
  public int field17;
  public int field18;
  public int field19;

  public void javaLoad(byte[] data)
  {
    field0 = data[76] | (data[77] << 8) | (data[78] << 16) | (data[79] << 24);
    field1 = data[72] | (data[73] << 8) | (data[74] << 16) | (data[75] << 24);
    field2 = data[68] | (data[69] << 8) | (data[70] << 16) | (data[71] << 24);
    field3 = data[64] | (data[65] << 8) | (data[66] << 16) | (data[67] << 24);
    field4 = data[60] | (data[61] << 8) | (data[62] << 16) | (data[63] << 24);
    field5 = data[56] | (data[57] << 8) | (data[58] << 16) | (data[59] << 24);
    field6 = data[52] | (data[53] << 8) | (data[54] << 16) | (data[55] << 24);
    field7 = data[48] | (data[49] << 8) | (data[50] << 16) | (data[51] << 24);
    field8 = data[44] | (data[45] << 8) | (data[46] << 16) | (data[47] << 24);
    field9 = data[40] | (data[41] << 8) | (data[42] << 16) | (data[43] << 24);
    field10 = data[36] | (data[37] << 8) | (data[38] << 16) | (data[39] << 24);
    field11 = data[32] | (data[33] << 8) | (data[34] << 16) | (data[35] << 24);
    field12 = data[28] | (data[29] << 8) | (data[30] << 16) | (data[31] << 24);
    field13 = data[24] | (data[25] << 8) | (data[26] << 16) | (data[27] << 24);
    field14 = data[20] | (data[21] << 8) | (data[22] << 16) | (data[23] << 24);
    field15 = data[16] | (data[17] << 8) | (data[18] << 16) | (data[19] << 24);
    field16 = data[12] | (data[13] << 8) | (data[14] << 16) | (data[15] << 24);
    field17 = data[8] | (data[9] << 8) | (data[10] << 16) | (data[11] << 24);
    field18 = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);
    field19 = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
  }

  public void javaLoadByteBuffer(byte[] data)
  {
    int ptr = data.length - 4;

    field0 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field1 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field2 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field3 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field4 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field5 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field6 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field7 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field8 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field9 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field10 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field11 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field12 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field13 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field14 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field15 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field16 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field17 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field18 = byte_buffer.getInt(ptr);
    ptr -= 4;
    field19 = byte_buffer.getInt(ptr);
    ptr -= 4;
  }

  public void reverseNums()
  {
    System.out.println("Wrong one!");
  }

  public void initByteBuffer(byte[] data)
  {
    byte_buffer = ByteBuffer.wrap(data);
    byte_buffer.order(ByteOrder.LITTLE_ENDIAN);
  }

  public void print()
  {
    System.out.println(field0);
    System.out.println(field1);
    System.out.println(field2);
    System.out.println(field3);
    System.out.println(field4);
    System.out.println(field5);
    System.out.println(field6);
    System.out.println(field7);
    System.out.println(field8);
    System.out.println(field9);
    System.out.println(field10);
    System.out.println(field11);
    System.out.println(field12);
    System.out.println(field13);
    System.out.println(field14);
    System.out.println(field15);
    System.out.println(field16);
    System.out.println(field17);
    System.out.println(field18);
    System.out.println(field19);
  }

  native void init();
  native void jniLoad(byte[] data);
  native static void timerStart();
  native static void timerStop();

  static
  {
    System.loadLibrary("testdata");
  }
}


