CC = gcc
CC_COMP_FLAG = -std=c99 -Wall -Wextra -Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

VPATH = graphics
EXEC = SPFIAR
GENERAL_OBJECTS = ArrayList.o Files.o GameBoard.o Pieces.o
GENERAL_HEADER  = DataDefinitions.h
MINIMAX_OBJECT  = MiniMax.o
CONSOLE_OBJECTS = ConsoleMode.o Parser.o
GUI_OBJECTS     = Button.o GameWindow.o GuiManager_Game.o GuiManager_Menu.o GuiManager.o SPCommon.o \
Windows_menu.o Windows.o
UNIT_TESTS = FilesTester ParserTester console_tester GameBoardTester Game_Window_Tester
# GUI_HELPERS_HEADERS = 
OBJ = 
$(EXEC): $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS)
	$(CC) $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(COMP_FLAG) $(SDL_LIB) -o $@

# testers-rules - should add more tester rules later on
FilesTester: $(GENERAL_OBJECTS)
	$(CC) $(GENERAL_OBJECTS) -o $@
Test.o: GameBoardTester.h ParserTester.h FilesTester.h ConsoleTester.h graphics/Game_Window_Tester.h graphics/GuiManager.h

# console & general rulse
ArrayList.o: SPArrayList.h Pieces.o
	$(CC) $(COMP_FLAG) -c $*.c
ConsoleMode.o: Parser.h DataDefinitions.h GameBoard.h MiniMax.h Files.h
	$(CC) $(COMP_FLAG) -c $*.c
Files.o: Files.h
	$(CC) $(COMP_FLAG) -c $*.c
GameBoard.o: GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
MiniMax.o: MiniMax.h GameBoard.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.h
	$(CC) $(COMP_FLAG) -c $*.c
Pieces.o: Pieces.h
	$(CC) $(COMP_FLAG) -c $*.c

# graphic related rules
Button.o: Button.h SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Game_Window_Tester.o: GameBoard.h Windows.h GameWindow.h GuiManager_Game.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameWindow.o: GameWindow.h SPCommon.h ConsoleMode.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiManager_Game.o: GameWindow.h SPCommon.h WindowDataStruct.h GuiManager_Game.h Windows.h ConsoleMode.h Files.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiManager_Menu.o: GuiManager_Menu.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GuiManager.o: GuiManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPCommon.o: SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Windows_Menu.o: Windows_Menu.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Windows.o: Windows.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

clean:
	rm -f *.o $(EXEC) $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS)
