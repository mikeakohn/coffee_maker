
CFLAGS=-O3 -Wall -g
INCLUDES=-I/opt/jdk1.6.0_39/include/ -I/opt/jdk1.6.0_39/include/linux
SEQUENCE=0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19

default:
	g++ -c ClassReader.cxx $(CFLAGS)
	g++ -c ClassWriter.cxx $(CFLAGS) -fPIC
	g++ -o coffee_dump coffee_dump.cxx ClassReader.o $(CFLAGS)
	g++ -o coffee_write coffee_write.cxx ClassWriter.o $(CFLAGS)
	g++ -o libcoffee_maker.so coffee_maker.cxx ClassWriter.o \
	  -shared -fPIC $(CFLAGS) $(INCLUDES)
	gcc -o libmyobject.so myobject.c -Wall -O3 -shared -fPIC $(INCLUDES)

test:
	./coffee_write TestClass.class
	javac Test.java
	java Test $(SEQUENCE)

jni:
	javac MyObject.java
	javac Test.java
	LD_LIBRARY_PATH=. java Test $(SEQUENCE)
	LD_LIBRARY_PATH=. java -Xcomp Test $(SEQUENCE)

clean:
	@rm -f *.o coffee_dump coffee_write *.class *.so
	@echo "Clean!"

