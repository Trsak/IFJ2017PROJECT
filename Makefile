TARGET   = ifj2017
TRG_TEST = tests
LEADER   = xbartl06

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
  $(filter-out $(SRCDIR)/main.c, $(SOURCES)) \
  $(wildcard $(TESTDIR)/*.c)
INC_DIRS = -I$(SRCDIR) -I$(TESTDIR)/src

all: $(TARGET) clean

.PHONY: pack
pack:
	@echo "\034[0;33m1. Running unit tests...\033[0m"
	@echo "-----------------------------------------"
	test
	@echo "\034[0;33m2. Trying to build the project...\033[0m"
	$(TARGET)
	@echo "\034[0;33m3. Removing binary file...\033[0m"
	clean
	@echo "\034[0;33m4. Removing binary file...\033[0m"

.PHONY: test
test: run_tests clean_tests

$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "\033[0;33mBinary file created...\033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run_tests
run_tests: CFLAGS = -std=c99 -Wall -Wextra -s
run_tests:
	@echo "\033[0;33mBuilding unit tests...\033[0m"
	@$(CC) -include $(TESTDIR)/src/unity_fixture_malloc_overrides.h $(CFLAGS) $(INC_DIRS) $(SRC_TEST) -o $(TRG_TEST)
	@echo "\033[0;33mRunning unit tests...\033[0m"
	- ./$(TRG_TEST)
	@echo "\033[0;33mUnit testing completed...\033[0m"

.PHONY: clean_tests
clean_tests:
	@echo "\033[0;33mCleaning after unit tests...\033[0m"
	@$(rm) $(TRG_TEST)

.PHONY: clean
clean:
	@echo "\033[0;33mCleaning object files...\033[0m"
	@$(rm) $(OBJECTS)
	@echo "\033[0;32mObject files cleaned!\033[0m"

.PHONY: remove
remove: clean
	@echo "\033[0;33mRemoving binary file...\033[0m"
	@$(rm) $(TARGET)
	@echo "\033[0;32mBinary file removed!\033[0m"
