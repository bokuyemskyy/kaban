#!/bin/bash

# Find all C++ source files and format them

find ./app -type f \( -name "*.cpp" -o -name "*.hpp" \) ! -path "*/generated/*" \
    -exec clang-format -i {} +

echo "Code formatting complete"