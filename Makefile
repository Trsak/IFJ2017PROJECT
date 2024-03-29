TARGET   = ifj2017
LEADER   = xbartl06

SRCDIR   = .
TMPDIR   = tmpdir
OBJDIR   = .

CC       = gcc
CFLAGS   = -std=c99 -I$(SRCDIR)/ -Wall -s -Wextra -c -Wno-unused-function -O3

LINKER   = gcc
LFLAGS   = -Wall -s -I$(SRCDIR)/ -lm

BLACK    = tput setaf 0
RED      = tput setaf 1
GREEN    = tput setaf 2
YELLOW   = tput setaf 3
BLUE     = tput setaf 4
MAGENTA  = tput setaf 5
CYAN     = tput setaf 6
WHITE    = tput setaf 7

RESET    = tput sgr0

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -rf

all: $(TARGET) clean

.PHONY: pack
pack: test doc
pack:
	@$(BLUE)
	@echo "[ Packing project ]"
	@$(YELLOW)
	@echo " -> Creating .zip file"
	@$(RESET)
	@zip $(LEADER).zip $(SOURCES) $(INCLUDES) rozsireni rozdeleni Makefile dokumentace.pdf > NUL
	@$(rm) NUL
	@$(GREEN)
	@echo " -> Zip archive created"
	@$(YELLOW)
	@echo " -> Running is_it_ok.sh"
	@$(RESET)
	sh is_it_ok.sh $(LEADER).zip $(TMPDIR)
	@$(YELLOW)
	@echo " -> Removing temp files"
	@$(RESET)
	@$(rm) $(TMPDIR)
	@$(BLUE)
	@echo "[ All done, check for errors and warnings! ]"
	@$(RESET)

.PHONY: test
test: $(TARGET) run_tests remove

$(TARGET): $(OBJECTS)
	@$(BLUE)
	@echo "[ Building project ]"
	@$(RESET)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@$(GREEN)
	@echo " -> Binary file \"$(TARGET)\" created"
	@$(RESET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: run_tests
run_tests:
	@$(BLUE)
	@echo "[ Preparing tests ]"
	@$(RESET)
	@sh ./tests/tests_runner.sh
	@$(RESET)

.PHONY: doc
doc:
	@$(BLUE)
	@echo "[ Creating documentation ]"
	@$(YELLOW)
	@echo " -> Creating dokumentace.pdf"
	@$(RESET)
	@$(MAKE) -C doc > NUL
	@$(rm) NUL
	@$(GREEN)
	@echo " -> dokumentace.pdf created"
	@$(RESET)

.PHONY: clean
clean:
	@$(BLUE)
	@echo "[ Cleaning object files ]"
	@$(RESET)
	@$(rm) $(OBJECTS)
	@$(GREEN)
	@echo " -> Object files cleaned"
	@$(RESET)

.PHONY: remove
remove: clean
remove:
	@$(BLUE)
	@echo "[ Removing all generated files ]"
	@$(YELLOW)
	@echo " -> Removing binary file"
	@$(RESET)
	@$(rm) $(TARGET)
	@$(YELLOW)
	@echo " -> Removing .zip file"
	@$(RESET)
	@$(rm) $(LEADER).zip
	@$(YELLOW)
	@echo " -> Removing temp dir and files"
	@$(RESET)
	@$(rm) $(TMPDIR)
	@$(rm) NUL
	@$(rm) ./tmp
	@$(rm) ./tmp.bas
	@$(rm) ./temp.code
	@$(GREEN)
	@echo " -> All generated files removed"
	@$(RESET)
