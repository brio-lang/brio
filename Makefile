# brio lang compiler makefile

CC := g++
CFLAGS := -std=c++1z
SRC := ./src/brio_lang/*.cc
TESTS := ./tests/brio_lang/*.cc
INCLUDE := -I./src/
LIBS := -lcurl -lfcgi++ -lfcgi

all:
	$(CC) $(CFLAGS) $(SRC) ./src/brio.cc $(INCLUDE) $(LIBS) -o ./bin/brio
test:
	$(CC) $(CFLAGS) $(SRC) $(TESTS) ./tests/main_test.cc $(INCLUDE) $(LIBS) -lgtest -pthread -o ./bin/tests
clean:
	$(RM) ./bin/brio
	$(RM) ./bin/tests
