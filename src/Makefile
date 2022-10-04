NAME = Project
CC = gcc -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
SOURSES = s21_string.c s21_sprintf.c
OBJECT = s21_string.o s21_sprintf.o
OS := $(shell uname -s)

ifeq ($(OS), Darwin)
 CC += -D OS_MAC
 TEST_FLAGS =-lcheck
endif
ifeq ($(OS), Linux)
 CC += -D OS_LINUX
 TEST_FLAGS =-lpthread -lcheck -pthread -lrt -lm -lsubunit
endif

all: clean s21_string.a test

$(NAME): $(OBJECT)
	$(CC) $(OBJECT) -o $(NAME)

s21_string.a: s21_string.o
	$(CC) -c $(SOURSES)
	@ar rcs s21_string.a $(OBJECT)

test: unit-test.c s21_string.a
	$(CC) $< s21_string.a $(TEST_FLAGS) -o unit-test

check: s21_string.a
	@cppcheck *.h *.c
	@cp ../materials/linters/CPPLINT.cfg CPPLINT.cfg
	@python3 ../materials/linters/cpplint.py --extension=c *.c
	@rm CPPLINT.cfg
	@$(CC) unit-test.c s21_string.a $(TEST_FLAGS)
	@CK_FORK=no leaks --atExit -- ./a.out

gcov_report: s21_string.a
	@gcc $(GCOVFLAGS) unit-test.c -lcheck $(SOURSES) $(TEST_FLAGS) -o GcovReport
	@./GcovReport
	@lcov -t "GcovReport" -o GcovReport.info -c -d .
	@genhtml -o report GcovReport.info
	@open ./report/index-sort-f.html
	@rm -rf ./*.gcno ./*.gcda ./GcovReport.*  

clean:
	@rm -rf ./*.o ./*.a ./a.out ./GcovReport ./*.gcno ./*.gcda ./report ./*.info ./string_o ./*.dSYM ./unit-test ./CPPLINT*

lo:
	for i in `seq 100 $(OP)`;	do ./unit-test; done;

valgrind_check: s21_string.a
	CK_FORK=no valgrind --tool=memcheck ./unit-test
	 
lik: s21_string.a
	CK_FORK=no leaks -atExit -- ./unit-test

rebuild:
	$(MAKE) clean
	$(MAKE) all
