COMPILER_CPP=g++
CPP_STD=-std=c++20
OPTIMIZATION_LEVEL=-O2
OPT=-fconstexpr-depth=700
INCLUDE_DIRS=-I./tests -I./lib/
LIB_FLAGS=
DEBUG_FLAGS=-fsanitize=undefined -fsanitize=address

ifeq ($(OS),Windows_NT)     # is Windows_NT on XP, 2000, 7, Vista, 10...
	LIB_FLAGS += -lwsock32 -lws2_32
else
    ifeq ($(UNAME_S),Darwin)
        LIB_FLAGS += -stdlib=libc++
    endif
endif

WARNING_FLAGS=-Wall -Wextra -pedantic

all: build_all run

test: clean build_tests run_tests

build_all: build # build_tests benchmark_build

build:
	$(COMPILER_CPP) $(CPP_STD) $(OPTIMIZATION_LEVEL) $(OPT) ./example/main.cpp -o ./bin/main.exe $(INCLUDE_DIRS) $(WARNING_FLAGS) $(LIB_FLAGS) 

build_tests:
	$(COMPILER_CPP) $(CPP_STD) $(OPTIMIZATION_LEVEL) $(OPT) ./tests/main.cpp -o ./bin/test.exe $(INCLUDE_DIRS) $(WARNING_FLAGS) $(LIB_FLAGS) 

debug_build_all: debug_build # debug_build_tests

debug_build:
	$(COMPILER_CPP) $(CPP_STD) $(OPTIMIZATION_LEVEL) $(OPT) ./example/Sample/main.cpp -o ./bin/main.exe $(INCLUDE_DIRS) $(WARNING_FLAGS) $(LIB_FLAGS) $(DEBUG_FLAGS) 

debug_build_tests:
	$(COMPILER_CPP) $(CPP_STD) $(OPTIMIZATION_LEVEL) $(OPT) ./tests/main.cpp -o ./bin/test.exe $(INCLUDE_DIRS) $(WARNING_FLAGS) $(LIB_FLAGS) $(DEBUG_FLAGS) 

clean:
	rm -rf ./bin
	mkdir -p ./bin/log

run:
	./bin/main.exe

run_tests:
	./bin/test.exe