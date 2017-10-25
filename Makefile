all: sortTester

sortTester : sortTester.o BucketSort.o
	g++ -std=c++14 -g -Wall -Werror -O2 -o sortTester sortTester.o BucketSort.o

sortTester.o: sortTester.cpp BucketSort.h
	g++ -std=c++14 -g -Wall -Werror -O2 -c sortTester.cpp

BucketSort.o : BucketSort.h BucketSort.cpp
	g++ -std=c++14 -g -Wall -Werror -O2 -c BucketSort.cpp

clean:
	rm *.o sortTester