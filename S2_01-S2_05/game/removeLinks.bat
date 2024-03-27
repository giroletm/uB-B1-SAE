@ECHO off

echo Removing existing links...

if exist android\app\src\main\assets\assets\ rmdir android\app\src\main\assets\assets
if exist android\app\jni\src\ rmdir android\app\jni\src
if exist android\app\jni\include\ rmdir android\app\jni\include
if exist android\app\jni\SDL\ rmdir android\app\jni\SDL
if exist android\app\jni\SDL_image\ rmdir android\app\jni\SDL_image
if exist android\app\jni\SDL_ttf\ rmdir android\app\jni\SDL_ttf
if exist android\app\jni\SDL_mixer\ rmdir android\app\jni\SDL_mixer

if exist windows\SAE201205\assets\ rmdir windows\SAE201205\assets
if exist windows\SAE201205\src\ rmdir windows\SAE201205\src
if exist windows\SAE201205\include\ rmdir windows\SAE201205\include

if exist switch\romfs\assets\ rmdir switch\romfs\assets
if exist switch\src\ rmdir switch\src
if exist switch\include\ rmdir switch\include

if exist windows\SAE201205\save.bin del windows\SAE201205\save.bin

if not defined callFlag (
	echo All done!
	pause
)
