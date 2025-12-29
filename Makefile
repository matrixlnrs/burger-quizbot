CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I./include
SRC = src/main.c src/admin.c src/database.c src/game.c src/utils.c
OBJDIR = obj
BINDIR = bin
# Transform source file names to object file paths in obj/ directory
OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(notdir $(SRC)))
# Search for .c source files in the src/ directory
vpath %.c src
EXEC = $(BINDIR)/burger-quizbot

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run