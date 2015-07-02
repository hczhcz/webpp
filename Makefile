CC = clang++

FLAGS = -Wall -Wextra -pedantic -ferror-limit=1 -std=c++11

CFLAGS = -I/media/hcz/B/code/projects/reflection++ \
    `pkg-config --cflags cgicc` \
    `pkg-config --cflags libmongocxx`

LFLAGS = -lfcgi -lfcgi++ \
    `pkg-config --libs cgicc` \
    `pkg-config --libs libmongocxx`

ALL_HPP = $(wildcard *.hpp)
ALL_CPP = $(wildcard *.cpp)
ALL_EXIST_FCGI = $(wildcard *.fcgi)
ALL_TARGET = $(patsubst %.cpp,%.fcgi,$(ALL_CPP))

default: compile

%.fcgi: %.cpp $(ALL_HPP)
	$(CC) $(FLAGS) $(CFLAGS) $(LFLAGS) $< -o $@

compile: $(ALL_TARGET)

run: compile
	sudo service apache2 restart

clean:
	touch rm_placeholder
	rm rm_placeholder $(ALL_EXIST_FCGI)
