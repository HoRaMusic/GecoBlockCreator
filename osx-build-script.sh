#!/bin/sh
DATE=`date +%Y-%m-%d`

# TODO:
QTDIR=~/Qt/5.8/clang_64/
echo $QTDIR
rm -r build-osx
mkdir build-osx
cd build-osx

$QTDIR/bin/qmake ../BlockModelCreator.pro -config release
make
$QTDIR/bin/macdeployqt BlockModelCreator.app -dmg

# output
cd ..

mkdir dist
cd dist
mkdir osx
cd osx
cp ../../build-osx/BlockModelCreator.dmg ./BlockModelCreator-$DATE.dmg
