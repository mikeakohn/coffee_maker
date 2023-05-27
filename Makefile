
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
	@rm -f python/*.so
	@rm -f coffee_dump coffee_write *.so
	@rm -f jni/*.class
	@rm -f sample/*.class
	@echo "Clean!"

