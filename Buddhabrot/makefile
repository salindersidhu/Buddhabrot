# Compiler and flag settings
CC=g++
CFLAGS=-std=c++0x -c
# Sources for compiling
SOURCES=ComplexNumber.cpp Buddhabrot.cpp main.cpp
# Objects from the above sources
OBJECTS=$(SOURCES:.cpp=.o)
# Name of the compiled binary
EXECUTABLE=Buddhabrot

# Build the project
all: $(OBJECTS) $(EXECUTABLE)

# Rules for compiling all sources
$(EXECUTABLE): $(OBJECTS)
	@printf "BUILDING PROJECT..."
	@$(CC) $(OBJECTS) -o $@
	@printf "DONE\n\nPlease type ./Buddhabrot to start the application.\n"

.cpp.o:
	@printf "COMPILING "$<"..."
	@$(CC) $(CFLAGS) $< -o $@
	@printf "DONE\n"

# Clean all object files and executable
.PHONY: clean
clean:
	@printf "CLEANING..."
	@rm -rf *.o $(EXECUTABLE)
	@printf "DONE\n"
