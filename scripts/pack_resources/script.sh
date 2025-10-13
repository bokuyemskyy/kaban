#!/bin/bash

read -p "Enter the output header filepath (e.g. resources.hpp): " header_file
read -p "Enter the resource directory filepath (e.g. resources/): " resource_dir

echo "#pragma once" > "$header_file"
echo "" >> "$header_file"

for file in "$resource_dir"/*; do
    if [[ -f "$file" && "$file" != "$0" ]]; then
        filename=$(basename "$file")
        var_name=$(echo "$filename" | sed 's/[^a-zA-Z0-9_]/_/g')

        {
            echo "// Generated from $filename"
            xxd -i -n "$var_name" "$file" | \
                sed 's/^unsigned char/inline constexpr unsigned char/' | \
                sed 's/^unsigned int/inline constexpr unsigned int/'
            echo ""
        } >> "$header_file"
    fi
done

echo "Header $header_file generated."
