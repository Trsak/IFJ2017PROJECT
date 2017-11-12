TARGET   = ifj2017
TRG_TEST = tests
LEADER   = xbartl06

SRCDIR   = .
TESTDIR  = test
TMPDIR   = tmpdir
OBJDIR   = .

CC       = gcc
CFLAGS   = -std=c99 -I$(SRCDIR)/ -Wall -s -Wextra -c -Wno-unused-function -O3

LINKER   = gcc
LFLAGS   = -Wall -s -I$(SRCDIR)/ -lm

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -rf

SRC_TEST = \
  $(wildcard $(TESTDIR)/src/*.c) \
  $(filter-out $(SRCDIR)/main.c, $(SOURCES)) \
  $(wildcard $(TESTDIR)/*.c)
INC_DIRS = -I$(SRCDIR) -I$(TESTDIR)/src

all: $(TARGET) clean

.PHONY: pack
pack: test $(TARGET) remove
pack:
	@echo -e "\033[0;33m [Creating .zip file...] \033[0m"
	@zip $(LEADER).zip $(SOURCES) $(INCLUDES) rozsireni rozdeleni Makefile dokumentace.pdf
	@echo -e "\033[0;32m [Zip archive created!] \033[0m"
	@echo -e "\033[0;33m [Running is_it_ok.sh...] \033[0m"
	sh is_it_ok.sh $(LEADER).zip $(TMPDIR)
	@echo -e "\033[0;33m [Removing temp files...] \033[0m"
	@$(rm) $(TMPDIR)
	@echo -e "\033[0;32m [All done, check for errors and warnings!] \033[0m"

.PHONY: test
test: run_tests clean_tests

$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo -e "\033[0;33m [Binary file created...] \033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run_tests
run_tests: CFLAGS = -std=c99 -Wall -Wextra -s
run_tests:
	@echo -e "\033[0;33m [Building unit tests...] \033[0m"
	@$(CC) -include $(TESTDIR)/src/unity_fixture_malloc_overrides.h $(CFLAGS) $(INC_DIRS) $(SRC_TEST) -o $(TRG_TEST)
	@echo -e "\033[0;33m [Running unit tests...] \033[0m"
	- ./$(TRG_TEST)
	@echo -e "\033[0;33m [Unit testing completed...] \033[0m"

.PHONY: clean_tests
clean_tests:
	@echo -e "\033[0;33m [Cleaning after unit tests...] \033[0m"
	@$(rm) $(TRG_TEST)

.PHONY: clean
clean:
	@echo -e "\033[0;33m [Cleaning object files...] \033[0m"
	@$(rm) $(OBJECTS)
	@echo -e "\033[0;32m [Object files cleaned!] \033[0m"

.PHONY: remove
remove: clean
remove:
	@echo -e "\033[0;33m [Removing all generated files...] \033[0m"
	@$(rm) $(TARGET)
	@$(rm) $(LEADER).zip
	@$(rm) $(TMPDIR)
	@echo -e "\033[0;32m [All generated files removed!] \033[0m"
