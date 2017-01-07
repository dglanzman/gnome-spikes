.PHONY: executables clean

executables: test xinput-ver

xinput-ver: xinput-ver.o
	cc -g $< -o $@ `pkg-config --libs gio-2.0 glib-2.0`

glib-valgrind-test: glib-valgrind-test.o
	cc -g $< -o $@ `pkg-config --libs gio-2.0 glib-2.0`

test: test.o
	cc -g $< -o $@ `pkg-config --libs glib-2.0`

%.o: %.c
	cc -c -g $< -o $@ `pkg-config --cflags gio-2.0 glib-2.0`

%.s: %
	objdump -D -S $< > $@

clean:
	rm -f xinput-ver
	rm -f glib-valgrind-test
	rm -f test
	rm -f *.s
	rm -f *.o

makefile: ;
