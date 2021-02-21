ImageOverlay.exe: main.o PGMImageData.o
	 g++ $^ -o $@

PGMImageData.o: PGMImageData.cpp PGMImageData.h Utils.h
	g++ -c PGMImageData.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	del *.o ImageOverlay.exe