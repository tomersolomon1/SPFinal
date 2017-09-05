# compiler and compiling flags
CC = gcc
#CC_COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors
CC_COMP_FLAG = -std=c99 
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

# vriables and adding graphics to the makefile's path
VPATH = graphics

EXEC = prog
# for now our "main" is in Test.c, to be changed later
GENERAL_OBJECTS = ArrayList.o Files.o GameBoard.o GameBasicBuildingBlocks.o MiniMax.o Test.o
CONSOLE_OBJECTS = ConsoleMode.o Parser.o
GUI_OBJECTS     =  Button.o Game_Window_Tester.o GameWindow.o GuiManager_Game.o GuiManager_Menu.o \
GuiManager.o SPCommon.o Windows_Menu.o Windows.o
UNIT_TESTS = FilesTester.o console_tester.o GameBoardTester.o Game_Window_Tester.o
#ALL_OBJECTS := $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(MINIMAX_OBJECT) $(MAIN_OBJECT) 

# the executable file
$(EXEC): $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(UNIT_TESTS)
	$(CC) $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(UNIT_TESTS) $(SDL_LIB) -o $@

# tester rules - should add more tester rules later on
FilesTester: $(GENERAL_OBJECTS)
	$(CC) $(CC_COMP_FLAG) -c $*.c
GameBoardTester.o: GameBoard.h Files.h
	$(CC) $(CC_COMP_FLAG) -c $*.c
Game_Window_Tester.o: GameBoard.h Windows.h GameWindow.h GuiManager_Game.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
console_tester.o: ConsoleMode.h GameBoard.h
		$(CC) $(CC_COMP_FLAG) -c $*.c
Test.o: GameBoardTester.h ParserTester.h FilesTester.h ConsoleTester.h Game_Window_Tester.h GuiManager.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

# console & general-objects & minimax rules
ArrayList.o: ArrayList.h GameBasicBuildingBlocks.o
	$(CC) $(CC_COMP_FLAG) -c $*.c
ConsoleMode.o: Parser.h DataDefinitions.h GameBoard.h MiniMax.h Files.h
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

# graphic related rules
graphics/Button.o: Button.h SPCommon.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
graphics/Game_Window_Tester.o: GameBoard.h Windows.h GameWindow.h GuiManager_Game.h
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
	rm -f *.o $(EXEC) $(UNIT_TESTS)
