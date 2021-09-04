CXXFLAGS = -g OBJ = Realloc.o Calloc.o Malloc.o Free.o memtest.o EXE =
    memtest

            $(EXE)
    : $(OBJ) g++ $(CXXFLAGS) - o $ @$ ^

    clean : rm - f $(OBJ) $(EXE)
