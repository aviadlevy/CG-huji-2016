aviadle
mosheperetz

EX1 part A
==============================================
--------------  Description  -----------------
==============================================

Model.cpp:
	We added few functions:
		createBall - responsible for creating another ball to our vector _balls with randomize color and randomize direction
		updateLocation - responsible for collisions (also call updateBall. see below).

Ball.cpp (new on part B):
	Constractor: when constructing a new ball, we checked the maximum radius possible considering walls and other balls
	updateBall: check for walls collision.

SimpleShader.vert:
	Added transform matrix responsible for scaling and transforming.

SimpleShader.frag:
	We've send the center of the ball, its radius, the window size and the light source location.
	then we calculate the center of the light, check the distance from each fragment, and then paint the ball.

Makefile:
	Changed from ex0 to ex1.

==============================================
----------------  Files  ---------------------
==============================================
	ex1.cpp                   -  the main program (not touched)
	ex1.h
	Makefile
	Model.cpp                 - responsible for the balls we draw
	Model.h
	Ball.cpp                  - responsible for each ball
	Ball.h
	Readme.txt                - THIS FILE
	ShaderIO.cpp              - responsible for the shaders behavior (not touched)
	ShaderIO.h
	shaders/SimpleShader.frag - the fragment shader
	shaders/SimpleShader.vert - the vertex shader