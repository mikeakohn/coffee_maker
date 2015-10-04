
CFLAGS=-O3 -Wall -g

default:
	g++ -c ClassReader.cxx $(CFLAGS)
	g++ -c ClassWriter.cxx $(CFLAGS)
	g++ -o coffee_dump coffee_dump.cxx ClassReader.o $(CFLAGS)
	g++ -o coffee_write coffee_write.cxx ClassWriter.o $(CFLAGS)

test:
	./coffee_write TestClass.class
	javac Test.java
	java Test

clean:
	@rm -f *.o coffee_dump coffee_write *.class
	@echo "Clean!"

