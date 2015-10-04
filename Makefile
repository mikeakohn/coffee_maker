
CFLAGS=-O3 -Wall -g

default:
	g++ -c ClassReader.cxx $(CFLAGS)
	g++ -c ClassWriter.cxx $(CFLAGS)
	g++ -o coffee_dump coffee_dump.cxx ClassReader.o $(CFLAGS)
	g++ -o coffee_maker coffee_maker.cxx ClassWriter.o $(CFLAGS)

clean:
	@rm -f *.o coffee_dump coffee_maker
	@echo "Clean!"

