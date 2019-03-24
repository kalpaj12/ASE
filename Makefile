CFLAGS = -std=c11 -Wall

all:
	clang $(CFLAGS) bind.c -c
	clang $(CFLAGS) glob.c -c
	clang $(CFLAGS) display.c -c
	clang $(CFLAGS) flags.c -c
	clang $(CFLAGS) main.c -c
	clang $(CFLAGS) mem.c -c
	clang $(CFLAGS) parse.c -c
	clang $(CFLAGS) stack.c -c
	clang $(CFLAGS) tengine.c -c

	clang bind.c display.o flags.o glob.o main.o mem.o \
	parse.o stack.o tengine.o -o ase

utests:
	@./tests.sh

clean:
	@rm *.o