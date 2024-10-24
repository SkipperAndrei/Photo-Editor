CC=gcc
CFRAGS=-Wall -Wextra-std-c=99

build:
	$(CC) $(CFLAGS) image_editor.c -o image_editor -lm
run: image_editor
	./image_editor
clean: 
	rm image_editor 
