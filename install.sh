#!/bin/bash
if [ "$EUID" -ne 0 ]
    then echo "Please run this script as root (using sudo)"
    exit
fi
execpath=$(realpath $(dirname "$0"))

echo "Installing dependencies..."
echo
add-apt-repository ppa:minhcly95/openea -y
apt update
apt install libboost1.63-all-dev libtinyxml2-dev libopenmpi-dev libeigen3-dev wcstools doxygen graphviz librestbed-dev
echo

echo "Copying CLI toolset..."
echo
ln -sf $(realpath "$execpath/EA-CLI/Exec/openea") /usr/local/bin/openea

echo "Compiling library..."
echo
runuser -l "$SUDO_USER" -c "$execpath/EA/pch.sh"
echo
runuser -l "$SUDO_USER" -c "openea update"

echo "Copying library files..."
echo
ln -sf $(realpath "$execpath/EA/SharedLib/libopenea.so") /usr/local/lib
rm -f /usr/local/include/openea
ln -sf $(realpath "$execpath/EA/src") /usr/local/include/openea

echo "Done. Run 'openea doc update' to generate doc files."
