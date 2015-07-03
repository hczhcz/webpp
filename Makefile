CC = clang++

FLAGS = -Wall -Wextra -pedantic -ferror-limit=1 -std=c++11

CFLAGS = -I/media/hcz/B/code/projects/reflection++ \
	`pkg-config --cflags openssl` \
	`pkg-config --cflags cgicc` \
	`pkg-config --cflags libmongocxx`

LFLAGS = -lfcgi -lfcgi++ \
	`pkg-config --libs openssl` \
	`pkg-config --libs cgicc` \
	`pkg-config --libs libmongocxx`

P_HPP = header/bookstore.hpp

ALL_HPP = $(wildcard header/*.hpp)
ALL_CPP = $(wildcard source/*.cpp)
ALL_EXIST_FCGI = $(wildcard *.fcgi)
ALL_TARGET = $(patsubst source/%.cpp, %.fcgi, $(ALL_CPP))

default: compile

$(P_HPP).pch: $(ALL_HPP)
	$(CC) -x c++-header $(FLAGS) $(CFLAGS) $(P_HPP) -o $@

%.fcgi: source/%.cpp $(P_HPP).pch
	$(CC) -include $(P_HPP) $(FLAGS) $(CFLAGS) $(LFLAGS) $< -o $@

compile: $(ALL_TARGET)

restart:
	sudo service apache2 restart

run: compile restart

clean:
	touch rm_placeholder
	rm rm_placeholder $(ALL_EXIST_FCGI)
	rm $(P_HPP).pch
