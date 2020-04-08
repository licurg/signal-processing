LIBS := -lglfw3 -lm -lGLU -lGL -lGLEW -lpthread -lX11 -ldl -lXrandr -lXxf86vm -lXinerama -lXcursor -lXi

generate: clean
	@mkdir -p bin

clean:
	@rm -rf bin

signal-processing: generate
	$(CC) -o bin/signal-processing signal-processing.c $(LIBS)