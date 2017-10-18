TARGET   = ifj2017

SRCDIR   = src
OBJDIR   = .

CC       = gcc
CFLAGS   = -std=c99 -I$(SRCDIR)/ -Wall -s -Wextra -c -Wno-unused-function -O3

LINKER   = gcc
LFLAGS   = -Wall -s -I$(SRCDIR)/ -lm

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


.PHONY: build
build: $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	clean

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)