# Coffee Maker

This is a collection of tools and JNI code demonstrating the ability
to build .class files in memory and load them into a running JVM.

Command line tools:

* coffee_dump: Dump a class file into text for debugging.
* coffee_write: Demonstration of using CoffeeWriter.cxx to generate a
  Java class.

JNI / Java code:

* CoffeeMaker.java/cxx: JNI interface into CoffeeWriter giving the ability
  for a Java program to generate a class file into a byte[] array.
* TestData.java/c: A test class with fields that can be set from either
  JNI or from a generated .class file. Demonstrates speed differences of
  each.

Test:
* Test.java sample program that uses TestData.

A full explanation of this program is here:

https://www.mikekohn.net/software/java_class_parser.php

Building
========

After cloning the repo, the tools can be build by typing:

    make
    make jni
    sh run_sample.sh

Running make will build coffee_dump and coffee_write. Doing make jni
will build the JNI library, the supporting Java files, and the sample
program. To do make JNI, the Makefile in the build/ directory requires
JNI_PATH to be set to the directory that holds the jni.h and such.
This also requires OpenJDK is installed. If OpenJDK (or JDK) is in
a different directory, JNI_PATH should be changed.

coffee_dump
===========

This is a pretty useful little program that will dump the contents of
a .class file. The original purpose was to help show what's inside of
a .class file while writing CoffeeWrite.cxx and later to verify that
the contents of files created by CoffeeWrite.cxx are correct.

This program dumps everything from the ConstantPool, Fields, Methods,
and anything else useful from a .class file. An example of how to use
it:

    ./coffee_dump jni/TestData.class

coffee_write
============

This is just an interface to CoffeeWrite.cxx, which could be used as
a library for generating .class files. To use it:

    ./coffee_write Out.class
    ./coffee_dump Out.class

libcoffee_maker.so
==================

This is a JNI library that is used with CoffeeMaker.java. This provides
a Java programmer with a way to create Java .class files in a byte[]
array in memory. The in memory Java class file can then be loaded into
the JVM. An example of how to do that is in sample/Test.java and in
TestData.java / libtestdata.so.

