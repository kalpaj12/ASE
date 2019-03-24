#!/bin/bash

echo
echo "Running unit tests"

fail=0

for file in tests/*.c;
do
	gcc "$file" flags.c glob.c mem.c parse.c stack.c
	./a.out


	if [ $? -eq 1 ]
	then
		echo "Unit test fail: $file"
		fail=$(( $fail + 1 ))
	fi
done

echo "Failed unit tests: $fail"
rm a.out