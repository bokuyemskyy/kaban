#!/bin/bash

read -p "Enter the input filepath (eg. perftsuite.epd): " input_file
read -p "Enter the output filepath (eg. perft.cpp): " output_file

echo -e '#include <gtest/gtest.h>\n\n#include "movegen.hpp"\n#include "position.hpp"\n\nTEST(Perft, PerftMegaset) {\n\tPosition pos;\n' > "$output_file"

while IFS= read -r line || [ -n "$line" ]; do
    [[ -z "$line" ]] && continue

    fen=$(echo "$line" | cut -d ';' -f1 | awk '{print $1, $2, $3, $4, $5, $6}')
    echo -e "\tpos.setFromFEN(\"$fen\");" >> "$output_file"

    rest=$(echo "$line" | cut -d ';' -f2-)
    IFS=';' read -ra parts <<< "$rest"
    for part in "${parts[@]}"; do
        key=$(echo "$part" | awk '{print $1}' | tr -d '[:space:]')
        val=$(echo "$part" | awk '{print $2}' | tr -d '[:space:]')
        if [[ $key =~ ^D[1-6]$ ]] && [[ $val =~ ^[0-9]+$ ]]; then
            depth=${key:1}
            echo -e "\tEXPECT_EQ(Movegen::perft(pos, $depth), $val);" >> "$output_file"
        fi
    done

    echo "" >> "$output_file"
done < "$input_file"

echo -e '}' >> "$output_file"

echo "Done!"