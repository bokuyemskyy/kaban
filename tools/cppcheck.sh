#!/bin/bash

# Run cppcheck on project

cppcheck --enable=all --project=build/compile_commands.json --error-exitcode=1 -i interface/gui/resources/generated --suppress=missingIncludeSystem  --check-level=exhaustive --disable=unusedFunction --suppress=variableScope --suppress=checkersReport