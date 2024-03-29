CC = g++

#SPACE RECOGNITION
null :=
space := ${null} ${null}
${space} := ${space}	# ${ } is a space. Neat huh?
#END OFSPACE RECOGNITION

EXEC_NAME = $(notdir $(subst ${ },_,$(shell pwd)) )
RMV = rm -f src/*.o

ifneq ($(wildcard /usr/include/GLFW),)
	DIR_PREFIX = /usr
else
	DIR_PREFIX = /usr/local
endif

CCFLAGS += -D LINUX

ifeq ($(OS),Windows_NT)
	#WINDOWS BUILD OPTIONS
	ifneq ($(wildcard C:/include/SFML),)
		DIR_PREFIX = /usr
	else
		DIR_PREFIX = /usr/local
	endif
	RMV = cmd //C del *.o
	EXEC_NAME = $(notdir $(subst ${ },_,$(shell cd)) ).exe
endif

DLIBS = $(DIR_PREFIX)/lib/
GL_INCLUDE_DIR = $(DIR_PREFIX)/include/GLFW
GLFLAGS = -lpthread -lglfw -lGL -lX11 -lXrandr -lXi -ldl 
CFLAGS = $(GLFLAGS) -I$(GL_INCLUDE_DIR) -Wall -O0

EXEC_NAME = AngelinoEngine # Fix, placeholder name until i find a way to handle spaces

.PHONY : all clean # Target that arent a file

all : $(EXEC_NAME) clean

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o) # Considera i file .cpp ma che che hanno come suffisso .o

$(EXEC_NAME) : $(OBJ) src/glad.o
	@echo "** Building main executable, aka $(EXEC_NAME) ..."
	$(CC) -L$(DLIBS) -o $@ $(OBJ) src/glad.o $(CFLAGS)

%.o: %.cpp src/glad.c
	@echo "** Building obj files..."
	$(CC) -c $< -o $@

bnr : 
	@echo "** Build start"
	make
	@echo "** Excecuting.."
	./$(EXEC_NAME)

clean :
	@echo "** Removing object files..."
	$(RMV)

clean_all :
	@echo "** Removing all generated files..."
	$(RMV) $(EXEC_NAME)
