//
//  ex2.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "ex2b.h"
#include "Model.h"
#include "ShaderIO.h"

/** Internal Definitions */

#define	WINDOW_SIZE         (600) // initial size of the window               //
#define	WINDOW_POS_X        (100) // initial X position of the window         //
#define	WINDOW_POS_Y        (100) // initial Y position of the window         //

#define RC_OK                 (0) // Everything went ok                       //
#define RC_INVALID_ARGUMENTS  (1) // Invalid arguments given to the program   //
#define RC_INPUT_ERROR        (2) // Invalid input to the program             //

#define	ARGUMENTS_PROGRAM     (0) // program name position on argv            //
#define	ARGUMENTS_INPUTFILE   (1) // given input file position on argv        //
#define	ARGUMENTS_REQUIRED    (2) // number of required arguments             //

/** Key definitions */

#define KEY_ANIMATE         ('a') // Key used to start animation              //
#define KEY_ESC            ('\e') // Key used to terminate the program - ESC  //
#define KEY_QUIT            ('q') // Key used to terminate the program        //
#define KEY_RESET           ('r') // Key used to reset the applied TX's	      //
#define KEY_RELOAD          ('l') // Key used to reload the shaders 	      //
#define KEY_WIREFRAME       ('w') // Key used to toggle wireframe rendering   //
#define KEY_PERSPECTIVE     ('p') // Key used to toggle perspective rendering //
#define KEY_NORMAL          ('n') // Key used to toggle normal estimation //
#define KEY_PHONG           ('1') // Key used to toggle Phong //
#define KEY_GOURAUD         ('2') // Key used to toggle Gouraud //
#define KEY_RGB             ('3') // Key used to toggle RGB //
#define KEY_INCREASE        ('=') // Key used to toggle RGB //
#define KEY_DECREASE        ('-') // Key used to toggle RGB //


/** display callback */
void display(void);

/** window reshape callback  */
void windowResize(int width, int height);

/** keyboard callback  */
void keyboard(unsigned char key, int x, int y);

/** mouse click callback */
void mouse(int button, int state, int x, int y) ;

/** mouse dragging callback  */
void motion(int x, int y) ;

/** timer callback */
void timer(int value) ;

void parsMesh(char *filePath);

/** Global variables */

int     g_nFPS = 0, g_nFrames = 0;              // FPS and FPS Counter
int     g_dwLastFPS = 0;                        // Last FPS Check Time
bool    g_startAnimation = false;
bool    g_duringAnimation = false;
bool    arcBallLeft = false, arcBallRight = false, arcBallMiddle = false;
int     last_x = 0;
int     last_y = 0;
int     cur_x = 0;
int     cur_y = 0;

// A global variable for our model (a better practice would be to use a singletone that holds all model):
Model _model(WINDOW_SIZE, WINDOW_SIZE);

/** main function */
int main(int argc, char* argv[])
{
	std::cout << "Starting ex2b..." << std::endl;
	
	// Initialize GLUT
    glutInit(&argc, argv) ;
#ifdef __APPLE__
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE) ;
#else
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
	glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutCreateWindow("CG Ex2 B");
	
	// Initialize GLEW
    glewExperimental = GL_TRUE;
    int glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
        exit(1);
    }
	
#ifdef __APPLE__
    GLint sync = 1;
    CGLSetParameter(CGLGetCurrentContext(), kCGLCPSwapInterval, &sync);
#endif
    glEnable(GL_DEPTH_TEST);

	// Set callback functions:
    glutDisplayFunc(display) ;
    glutReshapeFunc(windowResize) ;
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(100, timer, 0);   // uint millis int value


	// Init anything that can be done once and for all:
	_model.init(argv[1]);

	// Set clear color to black:
	glClearColor(0.0, 0.0, 0.0, 0.0);
		
	// Start events/drawing loop
	glutMainLoop();
	
	return 0;
}

void display(void)
{
	// Clear the screen buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _model.draw2dCircle();

	// Let the model to draw itself...
    _model.draw3dModel();
	
	// Swap those buffers so someone will actually see the results... //
    glutSwapBuffers();
}

// This method is called when the window is resized
void windowResize(int w, int h)
{
    // Update model to fit the new resolution
	_model.resize(w, h);
    
    // set the new viewport //
    glViewport(0, 0, w, h);
    
    // Refresh the display //
    glutPostRedisplay();
}

/********************************************************************
 * Function  :	keyboard
 * Arguments :	key : the key that was pressed
 *              x   : x value of the current mouse location
 *              y   : y value of the current mouse location
 * Returns   : n/a
 * Throws    : n/a
 *
 * Purpose   : This function handles all the keyboard input from the user.
 *             It supports terminating the application when the KEY_QUIT is pressed.
 *
 \******************************************************************/
void keyboard(unsigned char key, int x, int y)
{
    unsigned int lower_key = tolower(key);
	
    switch(lower_key)
    {
        case KEY_RESET:
            _model.reset();
            glutPostRedisplay();
            break;
        case KEY_RELOAD:
            // Reload the shading programs of the object
            // For use in a future exercise
            break;
        case KEY_WIREFRAME:
            _model.changeWire();
            glutPostRedisplay();
            break;
        case KEY_PERSPECTIVE:
            _model.changePerspective();
            glutPostRedisplay();
            break;
        case KEY_NORMAL:
            _model.setNormal();
            glutPostRedisplay();
            break;
        case KEY_PHONG:
            _model.setShading(PHONG_MODE);
            glutPostRedisplay();
            break;
        case KEY_GOURAUD:
            _model.setShading(GOURAUD_MODE);
            glutPostRedisplay();
            break;
        case KEY_RGB:
            _model.setShading(RGB_MODE);
            glutPostRedisplay();
            break;
        case KEY_INCREASE:
            _model.setSpec(INCREASE_MODE);
            glutPostRedisplay();
            break;
        case KEY_DECREASE:
            _model.setSpec(DECREASE_MODE);
            glutPostRedisplay();
            break;
        case KEY_ANIMATE:
            if (!g_duringAnimation) {
                g_startAnimation = true;
            }
            break;
        case KEY_QUIT:
        case KEY_ESC:
            // Terminate the program:
            exit(RC_OK);
            break;
        default:
            std::cerr << "Key " << lower_key << " undefined\n";
            break;
    }
    
    return;
}

/********************************************************************
 * Function  :   mouse
 * Arguments :   button  : the button that was engaged in some action
 *               state   : the new state of that button
 *               x       : x value of the current mouse location
 *               y       : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse actions.
 *
 \******************************************************************/
void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN) {
            arcBallLeft = true;
            _model.startRotate(x, y);
        }
        else {
            arcBallLeft = false;
            _model.stopRotate(x, y);
        }
    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN) {
            arcBallRight = true;
            _model.startMove(x, y);
        }
        else {
            arcBallRight = false;
            _model.stopMove(x, y);
        }
    }
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN) {
            arcBallMiddle = true;
            _model.startZoom(y);
        }
        else {
            arcBallMiddle = false;
            _model.stopZoom(y);
        }
    }

    return;
}


/********************************************************************
 * Function  :   motion
 * Arguments :   x   : x value of the current mouse location
 *               y   : y value of the current mouse location
 * Returns   :   n/a
 * Throws    :   n/a
 *
 * Purpose   :   This function handles mouse dragging events.
 *
 \******************************************************************/
void motion(int x, int y)
{
    if(arcBallRight)
    {
        _model.move(x, y);
    }
    else if(arcBallLeft)
    {
        _model.rotate(x, y);
    }
    else if(arcBallMiddle)
    {
        _model.zoom(y);
    }
    return;
}

static const float animationDurationInFrames = 300;

void timer(int value) {
    /* Set the timer to be called again in X milli - seconds. */
    if (g_startAnimation)
	{
        value = 0;
        g_duringAnimation = true;
        g_startAnimation = false;
    }
    
    glutTimerFunc(25, timer, ++value);   // uint millis int value
    
    if (g_duringAnimation) {
        float t = (float)value / (float)animationDurationInFrames;
        if (t > 1) {
            g_duringAnimation = false;
            return;
        }
        
        glutPostRedisplay();
    }
}
