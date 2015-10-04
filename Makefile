
CFLAGS=-O3 -Wall -g
INCLUDES=-I/opt/jdk1.6.0_39/include/ -I/opt/jdk1.6.0_39/include/linux

default:
	g++ -c ClassReader.cxx $(CFLAGS)
	g++ -c ClassWriter.cxx $(CFLAGS) -fPIC
	g++ -o coffee_dump coffee_dump.cxx ClassReader.o $(CFLAGS)
	g++ -o coffee_write coffee_write.cxx ClassWriter.o $(CFLAGS)
	g++ -o libcoffee_maker.so coffee_maker.cxx ClassWriter.o \
	  -shared -fPIC $(CFLAGS) $(INCLUDES)

test:
	./coffee_write TestClass.class
	javac Test.java
	java Test

jni:
	javac Test.java
	LD_LIBRARY_PATH=. java Test

clean:
	@rm -f *.o coffee_dump coffee_write *.class *.so
	@echo "Clean!"

