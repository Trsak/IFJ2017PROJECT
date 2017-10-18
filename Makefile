#General
TARGET = ifj2017
TAGET_TESTS = ifj-tests
C_COMPILER = gcc

#Roots
SOURCE_ROOT = ./src
INC_DIRS = -I$(SOURCE_ROOT)

TEST_ROOT = ./test
INC_DIRS_TESTS = -I$(SOURCE_ROOT) -I$(TEST_ROOT)/src

#Base flags
CFLAGS = -std=c99 -Wall -Wextra -Wno-unused-function -c
CFLAGS_RELEASE = -O3 -s

#Files for build
SRC_FILES = $(wildcard $(SOURCE_ROOT)/*.c)
OBJECTS := $(patsubst $(SOURCE_ROOT)/%.c, ./%.o, $(SRC_FILES))

#Files for tests
SRC_TEST_FILES=\
  $(TEST_ROOT)/src/unity.c \
  $(SOURCE_ROOT)/built_in.c \
  $(TEST_ROOT)/test_built_in.c
OBJECTS_TEST := $(patsubst $(SOURCE_ROOT)/%.c, ./%.o, $(SRC_TEST_FILES))

#Targets
.PHONY: build test clean

build: CFLAGS += $(CFLAGS_RELEASE)
build: CFLAGS += $(INC_DIRS)
build: $(OBJECTS)
	$(C_COMPILER) $^ -o $(TARGET)

test: CFLAGS += $(INC_DIRS_TESTS)
test: $(OBJECTS_TEST)
	$(C_COMPILER) $^ -o $(TAGET_TESTS)

clean:
	rm -f $(TARGET) $(TAGET_TESTS) $(OBJECTS) $(OBJECTS_TEST)