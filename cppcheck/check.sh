#!/bin/sh
cppcheck -j2\
	--std=c++20\
	--enable=all\
	--suppress=missingIncludeSystem\
	--includes-file=cppcheck/includes.list\
	--include=cppcheck/macros.h\
	--max-configs=1\
	"$@"
