#!/bin/sh

#set -x
date >> ~jgeisler/classes/cos381/logfile
echo `ls -ld | awk '{ print $3 }'`@`hostname` >> ~jgeisler/classes/cos381/logfile

cat - <<EOF
This script broke under student use in a way that wasn't visible during our
testing.  I have temporarily enabled read permissions on my implementation
files so that you can access them.  If you continue, you agree to not read the
files.  Your use of this program has been logged.

Press <Ctrl>-C now if you do not agree to abide by these restrictions.
EOF
sleep 15

trap '' INT QUIT TSTP

USER_FILES="ALU.cpp ALU.h CPU.cpp CPU.h DirectAddress.cpp DirectAddress.h Extender.h Gates.h Mux.h RegisterFile.cpp RegisterFile.h Shifter.h ValueSlice.h"

OWNER=`ls -ld | awk '{ print $3 }'`
GROUP=`ls -ld | awk '{ print $4 }'`

CWD=`pwd`
DIR=`mktemp -d /tmp/MIPS-simulator.XXXXX`
chmod 755 $DIR

echo "Setting up files..."
cd ~jgeisler/classes/cos381/MIPS-simulator/
cp Makefile *.cpp *.h $DIR

cd $DIR
echo "Making my files..."
CXXFLAGS=-O3 make all > /dev/null

cd $CWD
cp $USER_FILES $DIR 2> /dev/null

cd $DIR
echo "Making your files..."
CXXFLAGS=-g make all

echo "Cleaning up..."
chown $OWNER:$GROUP cpu Tests
make -s destroy

cd $CWD
echo "Your files are in $DIR"
