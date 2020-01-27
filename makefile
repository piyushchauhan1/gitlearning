output.o :
	g++ -g -std=c++11 opengl.cpp road.cpp vehicle.cpp -o output.out -lglut -lGLU -lGL

.PHONY : clean

clean :
	rm output.out
