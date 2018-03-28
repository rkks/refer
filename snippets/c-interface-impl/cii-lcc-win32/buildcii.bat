:- buildcii.bat 
:- Builds the CII libraries with lcc-win32 and GNU make
:- Rob Judd <judd@alphalink.com.au> 98/12/04

@echo off
:- Attempt to rename thread-nt.c (needed by DOS make utility)
if not exist src\threadnt.c ren src\thread-nt.c threadnt.c
:- Did we succeed ?
if not exist src\threadnt.c goto error

make -f makefile.lcc all
echo .
echo Ready to create library, hit enter to continue
pause
echo .

copy libcii.lst build
cd build

:- Kill the ones that are here already
if exist libcii.lib del libcii.lib
if exist libcii.dll del libcii.dll

:- Open files first to stop warnings
echo > libcii.lib
:- echo > libcii.dll

:- Make the static library
lcclib libcii.lib @libcii.lst

:- Make the .dll
:- lcclnk -dll -o libcii.dll @libcii.lst ..\src\libcii.def
:- implib libcii.dll
cd ..

make -f makefile.lcc all

goto end

:error
:- If this failed, we're not in NT anyhow but this solution
:- is much simpler than conditional compilation
echo Renaming of src\thread-nt.c for 8.3 compatibility failed
echo which means this is probably plain DOS. Please manually
echo rename src\thread-nt.c to src\threadnt.c to proceed

:- And we're outa here
:end
echo .
echo Finished !
@echo on
