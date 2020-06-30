project      = ducksoup

CPP          = g++
STD          = -std=c++17
OPTIM        = -O3

src_dir      = ./src
inc_dir      = ./include/$(project)
http_inc_dir = ./include/cpp-httplib
build_dir    = ./build
bin_dir      = ${build_dir}/bin
obj_dir      = ${build_dir}/obj
build_dirs   = ${obj_dir} ${bin_dir}
sources      = $(wildcard ${src_dir}/*.cpp)
objects      = $(subst .cpp,.o,$(subst ${src_dir},${obj_dir},${sources}))
executable   = ${bin_dir}/${project}

CFLAGS       = -Wall -Wpedantic ${STD} $(OPTIM)
INC          = -I $(inc_dir) -I ${http_inc_dir}

.PHONY: all run clean leak-test

all: $(executable)

run: $(executable)
	@$(^)

${executable}: ${objects} | ${bin_dir}
	${CPP} ${CFLAGS} -o ${@} ${^}

${obj_dir}/%.o: ${src_dir}/%.cpp | ${obj_dir}
	${CPP} ${CFLAGS} ${INC} -c ${<} -o ${@}

${build_dirs}:
	mkdir -p ${@}

clean:
	rm -rf $(build_dir)/*

leak-test:
	valgrind --leak-check=full --track-origins=yes $(executable)

help:
	@echo "Try one of the following make goals:"
	@echo " * all - build project"
	@echo " * run - execute the project"
	@echo " * clean - delete build files in project"
	@echo " * leak-test - begin a valgrind memory leak test"
