#!/bin/bash
dir="$(dirname "$0")"

REGEX_1="([0-9]+)/[0-9]+/([0-9]+)"
REPL_1="\1//\2"

REGEX_2="\b([0-9]+)\.0+\b"
REPL_2="\1"

REGEX_3="\b([0-9]+\.[0-9]*[1-9])0+\b"
REPL_3="\1"

REGEX_4="-0( |$)"
REPL_4="0\1"

COMMAND="s:$REGEX_1:$REPL_1:g; s/$REGEX_2/$REPL_2/g; s/$REGEX_3/$REPL_3/g; s/$REGEX_4/$REPL_4/g"

shopt -s globstar

for obj in "$dir"/**/*.obj; do
	content="$(grep -E '^(v|vn|f) ' "$obj" | sed -E "$COMMAND")"
	
	echo "$content" > "$obj"
	
	mtl="${obj%.obj}.mtl"
	[ -f "$mtl" ] && rm "$mtl"
done
