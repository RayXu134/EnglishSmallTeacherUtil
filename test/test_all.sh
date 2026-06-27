#!/bin/bash

export TARGET=./main
export TEST=test

cd "/home/pay/Documents/Code/C/20260619_EnglishSmallTeacherUtil"

source $TEST/split_functions.sh


# Rebuild the project

echo Cleaning...
make clean

echo Rebuilding...
make

horizontal_big_split


# Run tests

./$TEST/test_read.sh
horizontal_big_split

./$TEST/test_gen.sh
horizontal_big_split

./$TEST/test_help.sh
horizontal_big_split

echo Finished all tests
