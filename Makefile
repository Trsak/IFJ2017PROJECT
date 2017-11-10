TARGET   = ifj2017
TRG_TEST = tests

SRCDIR   = .
TESTDIR  = test
OBJDIR   = .

CC       = gcc
CFLAGS   = -std=c99 -I$(SRCDIR)/ -Wall -s -Wextra -c -Wno-unused-function -O3

LINKER   = gcc
LFLAGS   = -Wall -s -I$(SRCDIR)/ -lm

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f

SRC_TEST = \
  $(wildcard $(TESTDIR)/src/*.c) \
  $(filter-out src/main.c, $(SOURCES)) \
  $(wildcard $(TESTDIR)/*.c)
INC_DIRS = -Isrc -I$(TESTDIR)/src

all: $(TARGET) clean

.PHONY: test
test: run_tests clean_tests

$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run_tests
run_tests: CFLAGS = -std=c99 -Wall -Wextra -s
run_tests:
	@$(CC) -include $(TESTDIR)/src/unity_fixture_malloc_overrides.h $(CFLAGS) $(INC_DIRS) $(SRC_TEST) -o $(TRG_TEST)
	- ./$(TRG_TEST)

.PHONY: clean_tests
clean_tests:
	@$(rm) $(TRG_TEST)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@$(rm) $(TARGET)