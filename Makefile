
#CFLAGS=-O3 -Wall -g
#INCLUDES=-I/usr/lib/jvm/java-8-openjdk-amd64/include -I/usr/lib/jvm/java-8-openjdk-amd64/include/linux
SEQUENCE=0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19

default:
	@+make -C build

.PHONY: jni
jni:
	@+make -C build jni
	javac jni/CoffeeMaker.java
	javac sample/TestData.java
	javac -classpath jni:sample:. sample/Sample.java

clean:
	@rm -f build/*.o
	@rm -f coffee_dump coffee_write *.so
	@rm -f jni/*.class
	@rm -f sample/*.class
	@echo "Clean!"

