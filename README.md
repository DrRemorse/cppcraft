cppcraft
==========


Installation

Pull repo, compile for your OS using makefile

Win64:
Install mingw-builds (dual target) to a folder that makes sense, ie. c:\MingW64
Include c:\mingw64 in PATH enviroment variable (google it)
Open a console in the cppcraft repo folder, execute:
mingw32-make -j8

Should compile right out of the box for windows (And the .exe is static)

Linux64:
For linux you need to install xorg-dev, libbass for linux x64 and you need to compile GLFW3
BASS: www.un4seen.com    GLFW: http://glfw.org

(I don't have alot of experience with linux!)

The workspaces are for CodeLite IDE, however feel free to use any IDE you want to

