OBJ = main.o maze.o
EXEC = maze
TAGS = g++ -Wall -g

$(EXEC): $(OBJ)
	$(TAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(TAGS) -c $<

clean:
	rm -f $(OBJ) $(EXEC)
