CXXFLAGS=-Wall -O3
CFLAGS=-Wall -O3
VPATH=../src:../jni
JNI_PATH=/usr/lib/jvm/java-8-openjdk-amd64
INCLUDES= \
  -I$(JNI_PATH)/include \
  -I$(JNI_PATH)/include/linux

default: class_reader.o ClassWriter.o
	gcc -o ../coffee_dump ../src/coffee_dump.c class_reader.o $(CFLAGS)
	g++ -o ../coffee_write ../src/coffee_write.cxx ClassWriter.o $(CFLAGS)

jni: ClassWriter.o
	g++ -o ../libcoffee_maker.so ../jni/CoffeeMaker.cxx ClassWriter.o \
  	  -shared -fPIC -I../src \
	  $(CFLAGS) \
	  $(INCLUDES)
	gcc -o ../libtestdata.so ../sample/TestData.c \
	  -Wall -O3 -shared -fPIC \
	  $(INCLUDES)

%.o: %.cxx %.h
	g++ -c $< -o $*.o $(CXXFLAGS)

%.o: %.c
	gcc -c $< -o $*.o $(CFLAGS)

