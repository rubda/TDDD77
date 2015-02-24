build/test.o: src/test.c
	mkdir -p build
	gcc -o build/test src/test.c -lgsl -lgslcblas

test: build/test.o

clean:
	rm -rf build
