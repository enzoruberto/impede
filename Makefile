CC = gcc
TARGET ?= test
SOURCES ?= run_test.c ngtest.c difeq_test.c
# Add include paths with CPPFLAGS, and library paths with LDFLAGS.
CPPFLAGS ?=
CFLAGS ?= -g
LDFLAGS ?=
LDLIBS ?= -lngspice -lm
# Set SANITIZERS= to disable, or SANITIZERS=address,undefined.
SANITIZERS ?= address
SANFLAGS = $(if $(strip $(SANITIZERS)),-fsanitize=$(SANITIZERS))
OBJECTS := $(SOURCES:.c=.o)
DEPS := $(OBJECTS:.o=.d)
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
.PHONY: all run clean
all: $(TARGET)
$(TARGET): $(OBJECTS) Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) $(SANFLAGS) -o "$@" $(OBJECTS) $(LDLIBS)
$(OBJECTS): Makefile
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANFLAGS) -MMD -MP -c "$<" -o "$@"
	
run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	rm -f $(OBJECTS) $(DEPS) $(TARGET)
-include $(DEPS)
