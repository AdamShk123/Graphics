all: clean compile link

FILES := src/main.cpp src/glad.c


LIBS := -l SDL2main -l SDL2 -l SDL2_mixer -l GL

compile:
	g++ --std=c++17 -g ${FILES} -c
	mv *.o obj/

link:
	g++ obj/* -o main.exe ${LIBS}

clean:
	rm obj/* main.exe
