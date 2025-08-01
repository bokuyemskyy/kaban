#!/bin/bash

RESOURCE_DIR="resources"
HEADER_FILE="./resources.hpp"
SOURCE_FILE="./resources.cpp"

echo "#pragma once" >> $HEADER_FILE
echo "" >> $HEADER_FILE

echo "#include \"resources.hpp\"" > $SOURCE_FILE
echo "" >> $SOURCE_FILE

for FILE in *; do
    if [[ -f "$FILE" && "$FILE" != "$0" ]]; then
        FILENAME=$(basename "$FILE")
        VAR_NAME=$(echo $FILENAME | sed 's/[-.]/_/g')

        xxd -i "$FILE" | sed 's/unsigned/const unsigned/' >> "$SOURCE_FILE"

        echo "const extern unsigned char ${VAR_NAME}[];" >> $HEADER_FILE
        echo "const extern unsigned int ${VAR_NAME}_len;" >> $HEADER_FILE
        echo "" >> $HEADER_FILE
    fi
done

echo "Resources were generated."