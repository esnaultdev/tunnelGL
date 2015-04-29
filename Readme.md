TunnelGL
========

An ENSICAEN's School Project

Presentation
------------

"During a week, make a 3D game using OpenGL"

This game is a 3D Tunnel based runner.

Made by 5 students from second year at ENSICAEN, specialized in Image and Multimedia :
- Ao
- Goutye
- Rafaël Garcia
- Émilie Louvat
- Pierre Vandromme

For more details about the project, check out the final presentation we made for the project
in the folder doc/presentations (English and French available)

Requirements
------------

- cmake > 2.6
- glm
- OpenGL > 3.3
- Assimp
- GLFW3
- GLEW

Compilation (Unix)
-----------

To compile the project :

	mkdir build
	cd build
	cmake ..
	make

Compilation (Windows)
-----------

To compile the project :

	Rename the CMakeList.win.txt to CMakeList.txt
	md build
	cd build
	cmake .. -G "MinGW Makefiles"
	mingw32-make

Run (Unix)
-----------

While in the build folder:

	./tunnelGL

Run (Windows)
-----------

While in the build folder:
	
	tunnelGL.exe