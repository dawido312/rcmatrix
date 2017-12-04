rcmatrix: rcmatrix.o
	g++ -g -Wall -pedantic $^ -o $@

rcmatrix.o: rcmatrix.cpp rcmatrix.h
	g++ -g -c -Wall -pedantic $< -o $@

.PHONY: clean

clean:
	-rm rcmatrix.o rcmatrix
