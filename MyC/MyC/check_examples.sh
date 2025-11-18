    #!/usr/bin/env bash
set -euo pipefail

SRCDIR="./Examples"
WITNESS_DIR="../../Examples"
all_ok=1
found=0

for src in "$SRCDIR"/ex*.myc; do
  [ -e "$src" ] || continue
  found=1
  base=$(basename "$src" .myc)

  # run runComp so it reads the .myc in SRCDIR and writes the generated pcode into SRCDIR
  ./runComp "$SRCDIR/$base" >/dev/null 2>&1 || true

  gen="$SRCDIR/${base}_pcode.c"
  witness="$WITNESS_DIR/${base}_pcode.c"

  t1=$(mktemp)
  t2=$(mktemp)

  # prepare normalized files (remove // comments, trailing spaces and blank lines)
  if [ -f "$witness" ]; then
    sed 's,//.*$,,' "$witness" | sed 's/[[:space:]]*$//' | sed '/^[[:space:]]*$/d' > "$t1"
  else
    : > "$t1"
  fi

  if [ -f "$gen" ]; then
    sed 's,//.*$,,' "$gen" | sed 's/[[:space:]]*$//' | sed '/^[[:space:]]*$/d' > "$t2"
  else
    : > "$t2"
  fi

  if cmp -s "$t1" "$t2"; then
    printf "%s \033[32mVrai\033[0m\n" "$base"
  else
    printf "%s \033[31mFaux\033[0m\n" "$base"
    all_ok=0
  fi

  rm -f "$t1" "$t2"

done

if [ $found -eq 0 ]; then
  exit 1
fi

if [ $all_ok -eq 1 ]; then
  exit 0
else
  exit 2
fi
 
