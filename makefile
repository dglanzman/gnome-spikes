main: xinput_ver.c
	cc xinput_ver.c -o xinput_ver `pkg-config --cflags --libs gio-2.0 glib-2.0`

test: test.c
	cc test.c -o test `pkg-config --cflags --libs glib-2.0`
