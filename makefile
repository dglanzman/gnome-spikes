xinput-ver: xinput-ver.c
	cc -g xinput-ver.c -o xinput-ver `pkg-config --cflags --libs gio-2.0 glib-2.0`

test: test.c
	cc -g test.c -o test `pkg-config --cflags --libs glib-2.0`

clean:
	rm -f xinput-ver
	rm -f test
