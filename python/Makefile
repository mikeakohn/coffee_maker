C?=gcc
CXX?=g++
INCLUDES=-I/usr/include/python3.11 -I../src
CFLAGS=-Wall -O3 -g

OBJECTS= \
  ../build/ClassWriter.o

default:
	$(CXX) -o class_writer.so class_writer.cxx \
	  $(OBJECTS) \
	  -shared -fPIC $(CFLAGS) $(INCLUDES)

clean:
	@rm -f class_writer.so

