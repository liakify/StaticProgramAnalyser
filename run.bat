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
mkdir "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\IntelliTrace\x64\"
wget --no-check-certificate http://infocommsociety.com/~zhijian/cs3203/Microsoft.IntelliTrace.Profiler.dll
move Microsoft.IntelliTrace.Profiler.dll "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\IntelliTrace\x64\"
wget --no-check-certificate http://infocommsociety.com/~zhijian/cs3203/codecov.zip
mkdir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Team Tools\"
mkdir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Team Tools\Dynamic Code Coverage Tools\"
move codecov.zip "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Team Tools\Dynamic Code Coverage Tools\"
7z x "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Team Tools\Dynamic Code Coverage Tools\codecov.zip"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Team Tools\Dynamic Code Coverage Tools\"

cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "Team06/Code06/StartupSPASolution.sln" "/p:PlatformToolset=v141" "/p:UseEnv=true" "/p:Configuration=Debug" "/p:Platform=Win32" "/verbosity:minimal""

echo ########################### RUNNING UNIT TESTS ############################

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" Team06/Code06/Debug/UnitTesting.dll /EnableCodeCoverage

echo ######################## RUNNING INTEGRATION TESTS ########################

call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\IDE\CommonExtensions\Microsoft\TestWindow\vstest.console.exe" Team06/Code06/Debug/IntegrationTesting.dll /EnableCodeCoverage
