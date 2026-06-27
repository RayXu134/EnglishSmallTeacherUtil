#!/bin/bash

source $TEST/split_functions.sh

echo Running $0
horizontal_split

$TARGET read $TEST/test_read_input_file.ini

horizontal_split
echo Exiting
