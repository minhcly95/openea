#!/bin/bash
cd $(dirname "$0")
cd ../../EA/SharedLib
echo Updating libopenea.so...
echo
make all
if [[ $? != 0 ]]; then
	exit
fi
echo

cd ../../EA-CLI/add-on
echo Add-on Directory: $(pwd)
echo

echo Found:
find . -name "*.h" -o -name "*.hpp"
echo

find . -name "*.h" | sed -e "s/^\.\//#include \"..\/add-on\//" -e "s/$/\"/" > "../src/Add-on.h"
echo Header file created at $(realpath ../src/Add-on.h).
echo

echo Invoking Make...
echo
cd ../Exec
make all
echo Add-on updated.
echo
