call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=x86 -arch=x86 -winsdk=10.0.18362.0
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
wget --no-check-certificate https://infocommsociety.com/~zhijian/cs3203/cppunittest.zip
7z x cppunittest.zip
set "INCLUDE=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\include\;%INCLUDE%"
set "LIB=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\lib\;%LIB%"
del Team06/Code06/StartupSPASolution.sln
wget --no-check-certificate http://infocommsociety.com/~zhijian/cs3203/StartupSPASolution.sln
move StartupSPASolution.sln Team06\Code06\
wget --no-check-certificate https://github.com/cpplint/cpplint/archive/master.zip
7z x master.zip
dir
python cpplint-master/cpplint.py --linelength=1000 --filter=+build,-build/namespaces,-build/c++11,-build/c++14,-build/c++tr1,-legal,+readability,-runtime,+whitespace,-whitespace/ending_newline,-readability/namespace,-whitespace/blank_line --quiet Team06/Code06/source/* > cpplint.txt 2>&1
type cpplint.txt
type nul > blankfile.txt
fc cpplint.txt blankfile.txt > nul
if errorlevel 1 (
	exit 1
)

cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "Team06/Code06/StartupSPASolution.sln" "/p:PlatformToolset=v141" "/p:UseEnv=true" "/p:Configuration=Debug" "/p:Platform=Win32" "/verbosity:minimal""

if not exist "Team06/Code06/Debug/UnitTesting.dll" (
	exit 1
)

if not exist "Team06/Code06/Debug/IntegrationTesting.dll" (
	exit 1
)

if not exist "Team06/Code06/Debug/AutoTester.exe" (
	exit 1
)

if not exist "Team06/Code06/Debug/SPA.lib" (
	exit 1
)

echo.
echo ########################### RUNNING UNIT TESTS ############################

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" Team06/Code06/Debug/UnitTesting.dll

echo.
echo ######################## RUNNING INTEGRATION TESTS ########################

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" Team06/Code06/Debug/IntegrationTesting.dll
