#!/bin/bash

input_file="perftsuite.epd"

while IFS= read -r line || [ -n "$line" ]; do
    [[ -z "$line" ]] && continue

    fen=$(echo "$line" | cut -d ';' -f1 | awk '{print $1, $2, $3, $4, $5, $6}')
    echo "pos.setFromFen(\"$fen\");"

    rest=$(echo "$line" | cut -d ';' -f2-)
    IFS=';' read -ra parts <<< "$rest"
    for part in "${parts[@]}"; do
        key=$(echo "$part" | awk '{print $1}' | tr -d '[:space:]')
        val=$(echo "$part" | awk '{print $2}' | tr -d '[:space:]')
        if [[ $key =~ ^D[1-6]$ ]] && [[ $val =~ ^[0-9]+$ ]]; then
            depth=${key:1}
            echo "EXPECT_EQ(pos.perft($depth), $val);"
        fi
    done

    echo ""
done < "$input_file"