#!/bin/bash

cd build || exit 1
make || exit 1

for test_file in ../Tests/*.csv; do
    name=$(basename "$test_file" .csv)
    out_file="../Tests/${name}.out"
    
    echo "Running test $name ..."
    ./csv_printer "$test_file" "$out_file"
    
    if [ $? -eq 0 ]; then
        echo "  -> OK, output written to $out_file"
    else
        echo "  -> FAILED"
    fi
done