#!/bin/bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 <source_file> <input_file> [output_file]"
    exit 1
fi

source_file=$1
input_file=$2

if [ ! -f "$source_file" ]; then
    echo "Error: Source file not found: $source_file"
    exit 1
fi

if [ ! -f "$input_file" ]; then
    echo "Error: Input file not found: $input_file"
    exit 1
fi

output_file="out.rm"

if [ $# -eq 3 ]; then
    output_file=$3
fi

compiler="./compiler"

$compiler $source_file $output_file

echo "Compilation complete. Output file: $output_file"

./virtual-machine/maszyna-wirtualna < $input_file $output_file
