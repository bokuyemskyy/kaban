#!/bin/bash



read -p "Enter the target header filepath (eg. resources.hpp): " header_file
read -p "Enter the target source filepath (eg. resources.cpp): " source_file
read -p "Enter the resource directory filepath (eg. resources/): " resource_dir


echo "#pragma once" >> $header_file
echo "" >> $header_file

echo "#include \"resources.hpp\"" > $source_file
echo "" >> $source_file

for file in "$resource_dir"/*; do
    if [[ -f "$file" && "$file" != "$0" ]]; then
        filename=$(basename "$file")
        var_name=$(echo $filename | sed 's/[-.]/_/g')

        xxd -i -n "$var_name" "$file" | sed 's/unsigned/const unsigned/' >> "$source_file"

        echo "const extern unsigned char ${var_name}[];" >> $header_file
        echo "const extern unsigned int ${var_name}_len;" >> $header_file
        echo "" >> $header_file
    fi
done

echo "Resources were generated."