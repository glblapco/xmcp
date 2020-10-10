compile:
	gcc -o xmcp xmcp.c -L/usr/X11/lib -lX11

install:
	cp xmcp /usr/bin
