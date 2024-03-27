@echo off
setlocal EnableDelayedExpansion

REM Create the build folder if it doesn't exist
if not exist build\ mkdir build

REM Get the path we are running the script in
SET currPath=%cd%\

REM For each subfolder of the src folder, create a folder in the build folder for objects to be put in
for /R "src\" %%f in (.) do ( 
	SET "folder=%%f"				& REM Get the folder name
	SET "folder=!folder:~0,-1!"			& REM Remove the . at the end
    	SET "folder=!folder:%currPath%=!"		& REM Remove the current path part to make it relative
	SET "folder=!folder:src=build!"			& REM Replace src with build
	if not exist "!folder!" mkdir "!folder!"	& REM If no folder with such name exists, create it
)

REM Make the project. Change this path if you didn't install devkitPro in the default folder
C:\devkitPro\msys2\usr\bin\make.exe
pause