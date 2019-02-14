#! /bin/sh
echo "Setting env for Linux"
frameworkPath=`pwd`:`pwd`/framework:
export PATH=~/ORexxDev/Windows/full/sandbox/rick/tests/framework:$PATH
export LD_LIBRARY_PATH=`pwd`/bin/LINUX:$LD_LIBRARY_PATH
