
CFLAGS=-O3 -Wall

default:
	g++ -c ClassReader.cxx $(CFLAGS)
	g++ -c ClassWriter.cxx $(CFLAGS)
	g++ -o coffee_dump coffee_dump.cxx class_reader.o $(CFLAGS)
	g++ -o coffee_maker coffee_maker.cxx class_writer.o $(CFLAGS)

clean:
	@rm -f *.o coffee_dump coffee_maker
	@echo "Clean!"

