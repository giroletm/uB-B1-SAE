@ECHO off

set "callFlag=1"
call removeLinks.bat

echo Done.
echo.

echo Creating links...

mklink /D android\app\src\main\assets\assets ..\..\..\..\..\common\assets
mklink /D android\app\jni\src ..\..\..\common\src
mklink /D android\app\jni\include ..\..\..\common\include
mklink /D android\app\jni\SDL ..\..\..\libs\SDL
mklink /D android\app\jni\SDL_image ..\..\..\libs\SDL_image
mklink /D android\app\jni\SDL_ttf ..\..\..\libs\SDL_ttf
mklink /D android\app\jni\SDL_mixer ..\..\..\libs\SDL_mixer

mklink /D windows\SAE201205\assets ..\..\common\assets
mklink /D windows\SAE201205\src ..\..\common\src
mklink /D windows\SAE201205\include ..\..\common\include
mklink windows\SAE201205\save.bin ..\..\common\save.bin

mklink /D switch\romfs\assets ..\..\common\assets
mklink /D switch\src ..\common\src
mklink /D switch\include ..\common\include

echo All done!
echo.
pause