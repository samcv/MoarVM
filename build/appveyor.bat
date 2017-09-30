ECHO ON
IF "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2015" GOTO FIFTEEN
IF "%APPVEYOR_BUILD_WORKER_IMAGE%" == "Visual Studio 2017" GOTO SEVENTEEN
ECHO "APPVEYOR_BUILD_WORKER_IMAGE NOT SET TO 'Visual Studio 2015' or 'Visual Studio 2017'"
EXIT 1

:FIFTEEN
IF "%PLATFORM%" == "x64" GOTO FIFTEEN_X_SIX_FOUR
IF "%PLATFORM%" == "x86" GOTO FIFTEEN_X_EIGHT_SIX
ECHO "PLATFORM NOT SET TO x64 or x86"
EXIT 1

:FIFTEEN_X_SIX_FOUR
call "C:\Program Files\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.cmd" /x64
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86_amd64
EXIT

:FIFTEEN_X_EIGHT_SIX
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
EXIT

:SEVENTEEN
IF "%PLATFORM%" == "x64" GOTO SEVENTEEN_X_SIX_FOUR
IF "%PLATFORM%" == "x86" GOTO SEVENTEEN_X_EIGHT_SIX
ECHO "PLATFORM NOT SET TO x64 or x86"
EXIT 1

:SEVENTEEN_X_SIX_FOUR
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
EXIT
:SEVENTEEN_X_EIGHT_SIX
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
EXIT

