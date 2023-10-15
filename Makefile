all: clean compile link

FILES := src/main.cpp src/glad.c

LIBS := -l SDL2main -l SDL2 -l SDL2_image -l SDL2_mixer -l SDL2_ttf -lGL -l SDL2_gpu

compile:
	g++ -g ${FILES} -c
	mv *.o obj/

link:
	g++ obj/* -o main.exe ${LIBS}

clean:
	rm obj/* main.exe
