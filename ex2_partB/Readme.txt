aviadle
mosheperetz

EX2B
==============================================
--------------  Description  -----------------
==============================================

we added some keyboard function as the exercise specified.
we used the given vectors in order to represent all the shadings.
we also calculate the normal face or vertices in 2 functions (readPointsOfModelFaces and readPointsOfModelVert) and used
special struct we create to help the process of the normalization.
in the shaders we used some mathematical calculation from the Tirgulim and also found some help on the internet.

we call the executable file 'ex3' because this is the name of the exe in the school solution, and the exercise description
not specified any name. we hope this will not cause any problems.

==============================================
----------------  Files  ---------------------
==============================================
	ex2b.cpp                   -  the main program (not touched)
	ex2b.h
	Makefile
	Model.cpp                 - responsible for the shape we draw
	Model.h
	Readme.txt                - THIS FILE
	ShaderIO.cpp              - responsible for the shaders behavior (not touched)
	ShaderIO.h
	shaders/CircleShader.frag
	shaders/CircleShader.vert
	shaders/GouraudShader.frag
	shaders/GouraudShader.vert
	shaders/PhongShader.frag
	shaders/PhongShader.vert
	shaders/RGBShader.frag
	shaders/RGBShader.vert

===============================================
----------------   URLs   ---------------------
===============================================

http://www.learnopengl.com/#!Getting-started/Camera
http://www.learnopengl.com/#!Lighting/Basic-Lighting
http://www.learnopengl.com/#!Lighting/Materials
https://www.youtube.com/watch?v=6-9XFm7XAT8
https://www.opengl.org/sdk/docs/man2/xhtml/glDrawElements.xml