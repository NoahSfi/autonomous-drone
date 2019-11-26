CXX = g++
CONFIG = `pkg-config --cflags --libs opencv4`

camera: camera.o
	$(CXX) -o camera camera.o $(CONFIG)

camera.o: camera.hpp
	$(CXX) -c camera.cpp

clean:
	rm -f camera
	rm -f camera.o