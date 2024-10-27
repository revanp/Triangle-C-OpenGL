CC = gcc
SRC = main.c
LIBS = -lglfw -lGLEW -framework OpenGL
EXEC = gradient-triangle

$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(LIBS)
	chmod +x $(EXEC)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC)