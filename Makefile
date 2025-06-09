# C compiler
CC = gcc

# Compiler flags:
# -Wall: Enable all common warnings
# -Wextra: Enable extra warnings not enabled by -Wall
# -g: Add debugging information (useful for gdb)
# -O2: Optimization level 2 (can be omitted during development)
CFLAGS = -Wall -Wextra -g -O2
# For specific C standard compliance, e.g., C99:
# CFLAGS += -std=c99

# Output executable name
TARGETS = myls

# Default rule (usually the first one in the Makefile)
all: $(TARGETS)

# Rule to compile the program
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Rule to clean compiled files and executable
clean:
	rm -f $(TARGETS)

# Rule to install (simple example, may require sudo)
install: $(TARGETS)
	cp $(TARGETS) /usr/local/bin/

# Rule to uninstall
uninstall: $(TARGETS)
	rm -f /usr/local/bin/$(TARGETS)

# Mark rules that are not files
.PHONY: all clean install uninstall
