objects = main.cpp display/display.cpp
progName = visDisplay

includes = 	-Ilibs/glm/		\

libs=`sdl2-config --libs` -lSDL2 -L/usr/local/lib -lGL -lGLU -lGLEW $(includes)

all:
	@g++ $(objects) -o $(progName) -std=c++11 -Wl,-Bdynamic $(libs) -Wl,-Bstatic -L./argen/libs -largen -Wl,-Bdynamic
	@chmod +x visDisplay

cube:
	@./$(progName) example/cube/project.argen

slope:
	@./$(progName) example/slope/project.argen

wall:
	@./$(progName) example/wall/project.argen

blockOfFlats:
	@./$(progName) example/blockOfFlats/project.argen

clean:
	@rm visDisplay
