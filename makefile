CC = g++
CFLAGS = -O3 -std=c++11
EXEC = PuzzleGenerator

all:
	rm -f $(EXEC)
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp Puzzle.cpp PuzzleGenerator.cpp
	./$(EXEC) 5 5 1 4

# Uncomment to run for many grid sizes
# CC = g++
# CFLAGS = -O3 -std=c++11
# EXEC = PuzzleGenerator

# all:
# 	rm -f $(EXEC)
# 	$(CC) $(CFLAGS) -o $(EXEC) main.cpp Puzzle.cpp PuzzleGenerator.cpp
# 	./$(EXEC) 5 5 1 4
# 	./$(EXEC) 10 10 1 4
# 	./$(EXEC) 15 15 1 4
# 	./$(EXEC) 20 20 1 4
# 	./$(EXEC) 25 25 1 4
# 	./$(EXEC) 30 30 1 4
