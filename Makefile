CFLAGS=-g -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wextra -Wno-unused-parameter -Ideps/gc/include/ -iquote inc/

all: deps/gc/libgc.la libkari.a repl

clean:
	rm src/*.o
	rm repl/*.o
	rm libkari.a
	rm ikari

repl: libkari.a repl/ikari.o
	$(CC) -o ikari libkari.a repl/*.o

repl/%.o: repl/%.c inc/*.h

libkari.a: src/context.o src/dict.o src/kari.o src/kari_stdlib.o src/vec.o src/parser.o src/vm.o
	ar r libkari.a src/*.o deps/gc/*.o 2> /dev/null

src/%.o: src/%.c inc/%.h

deps/gc/libgc.la: deps/gc/Makefile
	make -C deps/gc all

deps/gc/Makefile:
	cd deps/gc; \
	./configure