#!/bin/sh

TEST=test

cd "/home/pay/Documents/Code/C/20260619_EnglishSmallTeacherUtil"

horizontal_split() {
	echo "--------------------" 
}


echo Running $0
horizontal_split

make clean
make -B
horizontal_split

./main gen output.ini < ./$TEST/test_input.txt
horizontal_split

cat output.ini
horizontal_split

echo Exiting
