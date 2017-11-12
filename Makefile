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
pack: test $(TARGET) remove
pack:
	@echo "\033[0;33mCreating .zip file...\033[0m"
	@zip $(LEADER).zip $(SOURCES) $(INCLUDES) rozsireni rozdeleni
	@echo "\033[0;32mZip archive created!\033[0m"
	@echo "\033[0;33mRunning is_it_ok.sh...\033[0m"
	@chmod +x $(LEADER).zip
	./is_it_ok.sh

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
	@echo "\033[0;33mRemoving all generated files...\033[0m"
	@$(rm) $(TARGET)
	@$(rm) $(LEADER).zip
	@echo "\033[0;32mAll generated files removed!\033[0m"
