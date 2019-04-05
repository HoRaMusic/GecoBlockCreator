REM DATE=`date +%Y-%m-%d`

For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)

set QTDIR=C:\Qt\Qt5.8.0\5.8\mingw53_32\bin
set TOOLS=C:\Qt\Qt5.8.0\Tools\mingw530_32\bin

echo %QTDIR%

rd build-win
mkdir build-win
cd build-win

%QTDIR%/qmake.exe ../BlockModelCreator.pro -config release
%TOOLS%/mingw32-make.exe
REM %QTDIR%/windeployqt release/BlockModelCreator.exe

cd ..

REM output:

mkdir dist
cd dist
mkdir win
cd win

mkdir BlockModelCreator%mydate%

copy ..\..\build-win\release\BlockModelCreator.exe BlockModelCreator%mydate%\BlockModelCreator.exe

%QTDIR%/windeployqt.exe BlockModelCreator%mydate%\BlockModelCreator.exe


cd ../..
