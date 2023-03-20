CC = g++
CXXFLAGS = -c

OUT_FILE = FallafelHelper

OBJECTS = dbc.o main.o
SRC = src
INCLUDES = -I$(SRC)/

$(OUT_FILE): $(OBJECTS)
		$(CC) $(OBJECTS) -o $(OUT_FILE)

dbc.o: $(SRC)/dbc/dbc.cpp $(SRC)/dbc/dbc.hpp
		$(CC) $(CXXFLAGS) $(INCLUDES) $(SRC)/dbc/dbc.cpp
main.o: $(SRC)/main/main.cpp $(SRC)/common/common.hpp $(SRC)/dbc/dbc.hpp $(SRC)/structs/spell.hpp
		$(CC) $(CXXFLAGS) $(INCLUDES) $(SRC)/main/main.cpp

clean:
		rm -rf *.o $(OUT_FILE)