import sys

fen = None
last_valid = None

for line in sys.stdin:
    line = line.strip()
    if line.startswith("engine.fromFen"):
        if fen and last_valid:
            print(fen)
            print(last_valid)
        fen = line
        last_valid = None
    elif line.startswith("EXPECT_EQ"):
        parts = line.split(",")
        if len(parts) >= 2:
            value_str = parts[1].strip().rstrip(");")
            value = int(value_str)
            if value < 1_000_000:
                last_valid = line

if fen and last_valid:
    print(fen)
    print(last_valid)
