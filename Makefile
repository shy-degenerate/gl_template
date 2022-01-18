CC=g++
DEPS=glfw3 gl
CFLAGS= -Wextra -Wall -std=c++17 `pkg-config --cflags $(DEPS)`
SRC=src/main.cpp \
	src/stb_image.cpp \
	src/glad.c \
	src/shader.cpp \
	src/camera.cpp

INCLUDE=-I./include

LIBS=`pkg-config --libs $(DEPS)` -lpthread -ldl

all: 
	$(CC) $(CFLAGS) $(SRC) $(INCLUDE) $(LIBS) -g -o c 
