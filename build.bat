@echo off

if %processor_architecture%==AMD64 (
	"%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" WinAPI.sln /m /t:Build /p:Configuration=Release;Platform=x86
	"%PROGRAMFILES(X86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" WinAPI.sln /m /t:Build /p:Configuration=Release;Platform=x64
) else (
	"%ProgramFiles%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" WinAPI.sln /m /t:Build /p:Configuration=Release;Platform=x86
	"%ProgramFiles%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" WinAPI.sln /m /t:Build /p:Configuration=Release;Platform=x64
)

PAUSE
