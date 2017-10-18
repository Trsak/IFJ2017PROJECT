TARGET   = ifj2017
TRG_TEST = tests

SRCDIR   = src
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
  $(TESTDIR)/extras/fixture/src/unity_fixture.c \
  $(filter-out src/main.c, $(SOURCES)) \
  $(wildcard $(TESTDIR)/*.c)
INC_DIRS = -Isrc -I$(TESTDIR)/src -I$(TESTDIR)/extras/fixture/src

all: $(TARGET) clean

.PHONY: test
test: run_tests clean_tests

$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run_tests
run_tests: CFLAGS = -std=c99 -Wall -Wextra
run_tests:
	$(CC) $(CFLAGS) $(INC_DIRS) $(SRC_TEST) -o $(TRG_TEST)
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