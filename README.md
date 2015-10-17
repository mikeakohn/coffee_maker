# coffee_maker
Testing the possibility of generating Java class files.

A full explanation of this program is here:

http://www.mikekohn.net/software/java_in_memory_class_reader.php

The idea here is this is a library that can be used to generate a Java class file in memory and either save it
to disk or actually load the program directly into the virtual machine.  The main code is written in C, but
I provided a JNI layer for Java.  This whole code-base could easily be converted to Java.

In a way, this library provides a way to make a Java JIT that compiles code into Java byte-code.  Later
the Java JIT can convert it to native code.

