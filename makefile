# compiler and compiling flags
CC = gcc

CC_COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

# vriables and adding graphics to the makefile's path
VPATH = graphics

# name of program
EXEC = chessprog

# Objects
GENERAL_OBJECTS = ArrayList.o Files.o GameBoard.o GameBasicBuildingBlocks.o MiniMax.o Main.o
CONSOLE_OBJECTS = ConsoleMode.o Parser.o
GUI_OBJECTS     =  Button.o GameWindow.o GuiManager_Game.o GuiManager_Menu.o \
GuiManager.o SPCommon.o Windows_Menu.o Windows.o
ALL_OBJECTS := $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(MINIMAX_OBJECT) 

# the executable file
$(EXEC): $(ALL_OBJECTS)
	$(CC) $(ALL_OBJECTS) $(SDL_LIB) -o $@

# console & general-objects & minimax rules
ArrayList.o: ArrayList.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
ConsoleMode.o: Parser.h ConsoleDataDefinitions.h GameBoard.h MiniMax.h Files.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
Files.o: Files.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
GameBasicBuildingBlocks.o: GameBasicBuildingBlocks.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
GameBoard.o: GameBoard.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
MiniMax.o: MiniMax.h GameBoard.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
Parser.o: Parser.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
Main.o: ConsoleMode.h GuiManager.h GameBoard.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

# graphic related rules
graphics/Button.o: Button.h SPCommon.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/GameWindow.o: GameWindow.h SPCommon.h ConsoleMode.h Files.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/GuiManager_Game.o: GameWindow.h SPCommon.h WindowDataStruct.h GuiManager_Game.h Windows.h ConsoleMode.h Files.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/GuiManager_Menu.o: GuiManager_Menu.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/GuiManager.o: GuiManager.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/SPCommon.o: SPCommon.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/Windows_Menu.o: Windows_Menu.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/Windows.o: Windows.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

# clean rule
clean:
	rm -f *.o $(EXEC)

# all rule
all: $(EXEC)
	