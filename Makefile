CC = clang++

FLAGS = -Wall -Wextra -pedantic -ferror-limit=1 -std=c++11

CFLAGS = -I. \
	`pkg-config --cflags openssl` \
	`pkg-config --cflags cgicc` \
	`pkg-config --cflags libmongocxx`

LFLAGS = -lfcgi -lfcgi++ \
	`pkg-config --libs openssl` \
	`pkg-config --libs cgicc` \
	`pkg-config --libs libmongocxx`

HPP_COMMON = \
	$(wildcard reflection++/*.hpp) \
	$(wildcard reflection++/visitor/*.hpp) \
	$(wildcard web++/*.hpp)

HPP_DEMO = $(wildcard demo/*.hpp)
CPP_DEMO = $(wildcard demo/*.cpp)
HPP_PCH = demo/bookstore_headers.hpp
FCGI_EXIST = $(wildcard demo/fcgi-bin/*.fcgi)
FCGI_DEMO = $(patsubst demo/%.cpp, demo/fcgi-bin/%.fcgi, $(CPP_DEMO))

default:
	echo "make demo_pch\nmake demo\nmake clean"

$(HPP_PCH).pch: $(HPP_COMMON) $(HPP_DEMO)
	$(CC) -x c++-header $(FLAGS) $(CFLAGS) $(HPP_PCH) -o $@

demo/fcgi-bin/%.fcgi: demo/%.cpp $(HPP_PCH).pch
	$(CC) -include $(HPP_PCH) $(FLAGS) $(CFLAGS) $(LFLAGS) $< -o $@

demo_pch: $(HPP_PCH).pch

demo: demo_pch $(FCGI_DEMO)
	sudo service apache2 restart

clean:
	touch rm_placeholder.out
	rm rm_placeholder.out $(FCGI_EXIST) $(HPP_PCH).pch
