CFLAGS=-g -pedantic -Wall -Wshadow -Wpointer-arith  -Wcast-qual -Wextra -Wc++-compat -Werror -Wno-unused-parameter -Ideps/gc/include/ -iquote inc/

all: deps/gc/libgc.la libkari.a repl

clean:
	rm src/*.o
	rm src/lib/*.o
	rm repl/*.o
	rm libkari.a
	rm ikari

repl: libkari.a repl/ikari.o
	$(CC) $(CFLAGS) -o ikari libkari.a repl/*.o

repl/%.o: repl/%.c inc/*.h

libkari.a:	src/context.o src/dict.o src/kari.o src/vec.o src/parser.o src/execute.o \
						src/lib/math.o src/lib/system.o src/lib/control.o src/lib/comparison.o \
						src/lib/string.o src/lib/array.o src/lib/dict.o
	ar r libkari.a src/*.o src/lib/*.o deps/gc/*.o 2> /dev/null

src/%.o: src/%.c inc/%.h

deps/gc/libgc.la: deps/gc/Makefile
	make -C deps/gc all

deps/gc/Makefile:
	cd deps/gc; \
	./configure