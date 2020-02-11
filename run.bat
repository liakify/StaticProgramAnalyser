call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=x86 -arch=x86
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
wget https://infocommsociety.com/~zhijian/cs3203/cppunittest.zip
7z x cppunittest.zip
dir
set "INCLUDE=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\include\;%INCLUDE%"
set "LIB=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\lib\;%LIB%"
cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "Team06/Code06/StartupSPASolution.sln" "/p:PlatformToolset=v141" "/p:UseEnv=true" "/p:Configuration=Debug" "/p:Platform=Win32""
