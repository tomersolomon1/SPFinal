CC = gcc
CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
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

$(EXEC): $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS)
	$(CC) $(GENERAL_OBJECTS) $(CONSOLE_OBJECTS) $(GUI_OBJECTS) $(COMP_FLAG) $(SDL_COMP_FLAG) -o $@
FilesTester: $(GENERAL_OBJECTS)
	$(CC) $(GENERAL_OBJECTS) -o $@

# should add more tester rules later on

ArrayList.o: SPArrayList.h Pieces.o
	$(CC) $(COMP_FLAG) -c $*.c

clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)

--------------------------------

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.c SPSimpleMainWindow.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
Widget.o: Widget.c Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SimpleButton.o: SimpleButton.c SimpleButton.h Widget.h SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPWindow.o: SPWindow.c SPWindow.h  
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPSimpleMainWindow.o: SPSimpleMainWindow.c SPSimpleMainWindow.h SPWindow.h SimpleButton.h Widget.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPCommon.o: SPCommon.c SPCommon.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
