##
# Author: @codesome (https://github.com/codesome)
# Github: https://github.com/codesome/go-channel-in-cpp
##

test: test.o
	g++ --std=c++11 -o test test.o -lpthread
	./test
	rm test

test.o: test.cpp
	g++ --std=c++11 -c test.cpp

clean:
	rm -f test.o

.PHONY = all test clean