:: add this to .bat to your path file C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64
cd .\build
cmake -G "NMake Makefiles" ..
nmake
::xcopy /s /F /y compile_commands.json ..\compile_commands\debug_x64.json
cd ..
