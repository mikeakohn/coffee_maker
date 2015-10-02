
CFLAGS=-O3 -Wall

default:
	gcc -c class_reader.c $(CFLAGS)
	gcc -o coffee_dump coffee_dump.c class_reader.o $(CFLAGS)

clean:
	@rm -f *.o coffee_dump
	@echo "Clean!"

