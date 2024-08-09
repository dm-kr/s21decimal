CC = gcc
COVDIR = test_info
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lcheck -lpthread -lrt -lm -lsubunit --coverage
GCOVFLAGS = -fprofile-arcs -ftest-coverage
LIBS = -lm
BRANCHES = --rc lcov_branch_coverage=1
SUPPRESS = >> /dev/null 2>&1

SRCS = $(wildcard *.c)
HEADERS = $(wildcard *.h)

OBJ_LIBRARY := $(patsubst %.c, %.o, $(SRCS))

all: clean s21_decimal.a

%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

s21_decimal.a: $(OBJ_LIBRARY) $(HEADERS)
	@$(CC) $(CFLAGS) -c $(SRCS)
	@ar rcs s21_decimal.a $(OBJ_LIBRARY)
	@ranlib s21_decimal.a
	@rm -rf $(OBJ_LIBRARY) *.o

test: s21_decimal.a tests/s21_decimal_test.c
	@mkdir -p $(COVDIR)
	@${CC} tests/*.c *.c $(LDFLAGS) -I. -o $(COVDIR)/test $(SUPPRESS)
	@$(COVDIR)/test

gcov_report: test
	@lcov --capture --directory $(COVDIR) --output-file $(COVDIR)/test.info $(BRANCHES) $(SUPPRESS)
	@lcov --remove $(COVDIR)/test.info '/usr/include/*' '*/tests/*' --output-file $(COVDIR)/test.info $(BRANCHES) $(SUPPRESS)
	@genhtml $(COVDIR)/test.info --output-directory coverage_report $(BRANCHES) >> $(COVDIR)/genhtml.log 2>&1
	@bash tests/coverage.sh $(COVDIR)
	@xdg-open coverage_report/index.html $(SUPPRESS)

clang:
	@clang-format -style=Google -n *.c *.h

fixclang:
	@clang-format -style=Google -i *.c *.h

valgrind:
	@valgrind --tool=memcheck --leak-check=yes --track-origins=yes --quiet $(COVDIR)/test

cppcheck:
	@cppcheck --enable=all *.c *.h --suppress=unusedFunction --suppress=missingIncludeSystem

clean:
	@rm -f *.o *.a $(COVDIR)/*.gcno $(COVDIR)/*.gcda $(COVDIR)/*.info *.out *.gcov s21_decimal.a m/main
	@rm -rf coverage_report $(COVDIR) .clang-format test $(COVDIR)/test

.PHONY: test gcov_report s21_decimal.a all clean
