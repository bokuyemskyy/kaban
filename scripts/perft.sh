#!/bin/bash

KABAN_PATH="/home/amon/Code/kaban/out/bin/debug/main"

DEPTH="$1"
FEN="$2"
MOVES="$3"

output=$(
    $KABAN_PATH <<EOF
uci
isready
position $FEN moves $MOVES
go perft $DEPTH
quit
EOF
)

echo "$output" | grep -E '^[a-h][1-8][a-h][1-8][nbrq]?: [0-9]+' | sed 's/://'
echo
echo "$output" | tr -d '\001' | grep -oP 'Perft\(\d+\): \K\d+'
