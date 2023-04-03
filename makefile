OBJDIR = build
CC = gcc
CFLAGS = -Wall -Werror -Wextra
CLIBS = -lSDL2 -lm
FILES = tetris.c main.c print_color.c menu_scene.c utils.c board.c global.c solo_scene.c tetromino.c button.c test.c text.c ai_scene.c ai.c
OBJECTS = $(addprefix $(OBJDIR)/, $(FILES:.c=.o))
SRC_DIR = src

ifeq ($(OS), Windows_NT)
	REM = del /Q $(OBJDIR)\*
	EXEC = main.exe
else
	REM = rm $(OBJDIR)/*
	EXEC = ./main
endif

all: $(OBJDIR) main

$(OBJDIR):
	mkdir $(OBJDIR)

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ $(OBJECTS) $(CLIBS)

run: 
	cd $(OBJDIR) && $(EXEC)

$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< $(CLIBS)

clean:
	$(REM)