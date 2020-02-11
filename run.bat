call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=x86 -arch=x86 -winsdk=10.0.18362.0
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
wget https://infocommsociety.com/~zhijian/cs3203/cppunittest.zip
7z x cppunittest.zip
set "INCLUDE=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\include\;%INCLUDE%"
set "LIB=C:\Users\travis\build\nus-cs3203\team06-win-spa-19s2\CppUnitTest\lib\;%LIB%"
del Team06/Code06/StartupSPASolution.sln
wget http://infocommsociety.com/~zhijian/cs3203/StartupSPASolution.sln
move StartupSPASolution.sln Team06\Code06\
cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "Team06/Code06/StartupSPASolution.sln" "/p:PlatformToolset=v141" "/p:UseEnv=true" "/p:Configuration=Debug" "/p:Platform=Win32" "/verbosity:minimal""
