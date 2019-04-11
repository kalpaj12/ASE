CFLAGS = -std=c11 -Wall

all:
	gcc $(CFLAGS) a_math.c -c
	gcc $(CFLAGS) bind.c -c
	gcc $(CFLAGS) glob.c -c
	gcc $(CFLAGS) display.c -c
	gcc $(CFLAGS) flags.c -c
	gcc $(CFLAGS) main.c -c
	gcc $(CFLAGS) mem.c -c
	gcc $(CFLAGS) parse.c -c
	gcc $(CFLAGS) stack.c -c
	gcc $(CFLAGS) tengine.c -c

	gcc a_math.c bind.c display.o flags.o glob.o main.o mem.o \
	parse.o stack.o tengine.o -o ase

utests:
	@./tests.sh

clean:
	@rm *.o