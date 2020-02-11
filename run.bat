call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -host_arch=amd64 -arch=amd64
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\Common7\Tools\VsDevCmd.bat" -test
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
set "INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\VS\include\;%INCLUDE%"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\VS\"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\VS\include\"
cmd.exe /c ""C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\MSBuild\15.0\Bin\MSBuild.exe" "Team06/Code06/StartupSPASolution.sln" "/p:PlatformToolset=v141""
