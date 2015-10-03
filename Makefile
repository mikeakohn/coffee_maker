
CFLAGS=-O3 -Wall

default:
	gcc -c class_reader.c $(CFLAGS)
	gcc -c class_writer.c $(CFLAGS)
	gcc -o coffee_dump coffee_dump.c class_reader.o $(CFLAGS)
	gcc -o coffee_maker coffee_maker.c class_writer.o $(CFLAGS)

clean:
	@rm -f *.o coffee_dump coffee_maker
	@echo "Clean!"

