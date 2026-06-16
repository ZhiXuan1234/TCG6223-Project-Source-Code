/*
 TCG6223 Computer Graphics
 FIST, Multimedia University

 File: CNAmain.cpp
 Objective: Main Program for all file in the project

 Reference code from:
 Copyright (C) by Ya-Ping Wong <ypwong@mmu.edu.my>

 This file (CGLabmain.cpp) can be distributed to the students

 INSTRUCTIONS
 ============
 How to compile and run:
 * For each of the lab, you need the files below:

   a. CNAmain.cpp => The file you are reading now

   b. CNAmain.hpp => Header file to be used with CNAmain.cpp

   c. xx.cpp => Program for lab number 'xx', 'xx' being the lab
                     number such as 01, 03 ... 10, 11

   d. xx.hpp => Header file to be used with CGLabxx.cpp

 * Make sure you are including the correct include file
   in CNAmain.cpp (this file) such as:
      #include "xx.hpp"   where 'xx' is the function of the file

 * Make sure gl.h, glu.h and glut.h are in the 'include' path

 * Make sure opengl32.dll, glu32.dll and glut32.dll are in the 'system32' path

 * Make sure libopengl32.a, libglu32.a and libglut32.a are in the
   'lib' path and included in your project file if you are using an IDE

 * To compile using a command line using the GCC, type the command as below:
      g++ CNAmain.cpp CNAxx.cpp -o CNAxx.exe -lopengl32 -lglu32 -lglut32
      [replace 'xx' with the function of the file such as ObjModel, Kinger, GameLogic]

 How to modify:
 =============
 * All the user-defined drawing functions are called from
   the draw() function of class MyVirtualWorld which
   must be declared in CNAxx.hpp
 * myvirtualworld is declared in CNAmain.cpp as an instance
   of class MyVirtualWorld
 * MyVirtualWorld must implement the following member functions:
   a. draw()     => will be called by myDisplayFunc() of CNAmain.cpp
   b. tickTime() => will be called by myDisplayFunc() of CNAmain.cpp
                    for animation purposes
   c. init()     => will be called by myInit() of CNAmain.cpp
                    to carry up one-time setup/initialization before
                    any rendering BUT after OpenGL has been initialized
 * All classes and variables for each lab are defined in their own
   namespace to avoid names clashing of variable and type names.
   Specifically each CGLabxx.hpp declared and defined a MyVirtualWorld
   class, thus in CGLabmain.cpp, you need to specify from which
   namespace the MyVirtualWorld that you wish to use. Thus, you will
   need to modify the line (in the beginning of this program):
      CNAxx::MyVirtualWorld myvirtualworld;
   and change the 'xx' to the lab number that you wish to use.

   Game and 3d model by
   Group C&A


*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <windows.h>
#include <GL/glut.h>

#include "CNAmain.hpp"

//Ideally, you should include only the files that you wish
//  to use, however, all of them are included here just for
//  convenience.
#include "CNAWorld.hpp"

//IMPORTANT:
//  Change the namespace scope below corresponding to
//  to the lab number which you wish to use.
using ProjectWorld::MyVirtualWorld;

MyVirtualWorld myvirtualworld;

using namespace std;

MyWindow   window;
MyWorld    world;
MyViewer   viewer;
MySetting  setting;
MyAxis     worldaxis;

// ==========================================================================
// TPS Camera globals
// All tunable values are named constants — no hard coding.
// ==========================================================================
static const float CAMERA_DISTANCE           = 60.0f;  // units BEHIND Kinger (must be positive)
static const float CAMERA_BASE_HEIGHT        = 30.0f;  // extra height above the target point
static const float CAMERA_SHOULDER_OFFSET    = 18.0f;  // Over-the-shoulder offset to the right
static const float CAMERA_TARGET_HEIGHT_OFFSET = 5.0f; // Kinger's visual centre above Y=0 (chest level)
static const float CAMERA_PITCH_MAX          =  1.2f;  // radians (~69 deg, camera high up)
static const float CAMERA_PITCH_MIN          = -1.2f;  // radians (steep upward aiming allowed)
static const float MOUSE_SENSITIVITY         =  0.003f;
static const float KINGER_MOVE_SPEED         =  1.0f;  // world units per key press
static const float CAMERA_KEY_TURN_INC       =  0.04f; // radians per arrow key

float cameraYaw   = 0.0f;  // horizontal camera angle, radians
float cameraPitch = 0.4f;  // vertical camera angle, radians (positive = camera sits higher, looking down)

// Task 1: Add Camera Tracking State
float cameraTrackY = -18.7f;
const float CAMERA_FOLLOW_SPEED = 10.0f;

// Tracks which keys are currently held down.
// Indexed by the ASCII value of the key (0–255).
// Set to true in myKeyboardFunc, cleared in myKeyboardUpFunc.
bool keyStates[256] = {false};
// ==========================================================================

// --------------------------------------------------------------------------
// drawCrosshair — renders a 2D aiming reticle over the 3D scene.
// We disable depth testing and switch to an orthographic projection so
// the crosshair stays locked to the center of the window.
// --------------------------------------------------------------------------
void drawCrosshair()
{
    glDisable(GL_DEPTH_TEST);

    // Switch to 2D UI projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window.width, 0, window.height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float cx = (window.width / 2.0f)-15;
    float cy = (window.height / 2.0f)+30;
    float size = 10.0f;

    // Draw a simple white cross
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(cx - size, cy);
        glVertex2f(cx + size, cy);
        glVertex2f(cx, cy - size);
        glVertex2f(cx, cy + size);
    glEnd();

    glPopMatrix();

    // Restore 3D projection
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void updateKeyStatesFromWindows()
{
    keyStates['w'] = keyStates['W'] = (GetAsyncKeyState('W') & 0x8000) != 0;
    keyStates['a'] = keyStates['A'] = (GetAsyncKeyState('A') & 0x8000) != 0;
    keyStates['s'] = keyStates['S'] = (GetAsyncKeyState('S') & 0x8000) != 0;
    keyStates['d'] = keyStates['D'] = (GetAsyncKeyState('D') & 0x8000) != 0;

    // Left mouse button for auto shooting
    keyStates[1] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

void myDisplayFunc(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // ------------------------------------------------------------------
 // Follow camera: rebuild the view matrix every frame so the camera
 // always sits behind and above Kinger at the current cameraYaw/Pitch.
 //
 // kinger.posY is stored as -18.7f (ground level) and is used only as
 // a metadata value.  Kinger is actually DRAWN at Y = 0 (see Kinger::draw).
 // We therefore build both the look-at target and the eye position
 // from Y = 0 + CAMERA_TARGET_HEIGHT_OFFSET, which puts them at
 // Kinger's visual chest/centre rather than underground.
 //
 // Eye position (spherical orbit around the target point):
 //   eyeX = posX + sin(yaw)  * cos(pitch) * distance
 //   eyeY = targetY + CAMERA_BASE_HEIGHT + sin(pitch) * distance
 //   eyeZ = posZ + cos(yaw)  * cos(pitch) * distance
 // ------------------------------------------------------------------
 // ------------------------------------------------------------------
 float& kX = myvirtualworld.kinger.posX;
 float& kZ = myvirtualworld.kinger.posZ;

 // --- Task 2: Smooth Interpolation Math ---
 // Calculate deltaTime for the camera interpolation
 static int lastTime = glutGet(GLUT_ELAPSED_TIME);
 int currentTime = glutGet(GLUT_ELAPSED_TIME);
 float deltaTime = (currentTime - lastTime) / 1000.0f;
 if (deltaTime > 0.1f) deltaTime = 0.1f;
 lastTime = currentTime;

 // Smoothly interpolate cameraTrackY toward Kinger's true posY
 cameraTrackY += (myvirtualworld.kinger.posY - cameraTrackY) * CAMERA_FOLLOW_SPEED * deltaTime;

 // --- Task 3: Update gluLookAt Math ---
 // True visual centre of Kinger in world space.
 // Because Kinger's mesh is offset by +18.7f relative to his raw posY, we must add 18.7f to cameraTrackY here.
 const float baseTargetY = (cameraTrackY + 18.7f) + CAMERA_TARGET_HEIGHT_OFFSET;

 // Calculate the camera's local "Right" vector (perpendicular to forward)
 // We use this to shift the camera over Kinger's right shoulder.
 float rightX = std::cos(cameraYaw);
 float rightZ = -std::sin(cameraYaw);

 // Apply the shoulder offset to the target
 float targetX = kX + rightX * CAMERA_SHOULDER_OFFSET;
 float targetZ = kZ + rightZ * CAMERA_SHOULDER_OFFSET;

 float cosPitch = std::cos(cameraPitch);
 float sinPitch = std::sin(cameraPitch);

 // Calculate eye position orbiting around the newly offset target
 float eyeX = targetX + std::sin(cameraYaw) * cosPitch * CAMERA_DISTANCE;
 float eyeY = baseTargetY + CAMERA_BASE_HEIGHT + sinPitch * CAMERA_DISTANCE;
 float eyeZ = targetZ + std::cos(cameraYaw) * cosPitch * CAMERA_DISTANCE;

 // Prevent the camera from dipping below the floor.
 // kinger.posY stores the ground level metadata (-18.7f).
 const float GROUND_LEVEL = myvirtualworld.kinger.posY;
 if (eyeY < GROUND_LEVEL + 1.0f)
 {
     eyeY = GROUND_LEVEL + 1.0f;
 }

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(
     eyeX,    eyeY,        eyeZ,    // eye: behind, above, and to the right
     targetX, baseTargetY, targetZ, // target: offset to the right
     0.0f,    1.0f,        0.0f     // world up vector
 );
 // ------------------------------------------------------------------

 glPushMatrix();

    worldaxis.draw();

    myvirtualworld.draw();

 glPopMatrix();

 // Draw 2D UI elements last
 drawCrosshair();

 glFlush();
 glutSwapBuffers();

 updateKeyStatesFromWindows();
 myvirtualworld.tickTime(cameraYaw, cameraPitch, keyStates);
 glutPostRedisplay();
}

void myReshapeFunc(int width, int height)
{
 window.width  = width;
 window.height = height;
 glViewport(0, 0, width, height);
}

// --------------------------------------------------------------------------
// myKeyboardFunc — key DOWN: record state; handle one-shot actions.
// Movement (W/A/S/D) is intentionally NOT handled here; it is evaluated
// every frame inside kinger.update() via the keyStates array so that
// multiple simultaneous keys all register correctly.
// --------------------------------------------------------------------------
void myKeyboardFunc(unsigned char key, int x, int y)
{
 // Always record that this key is now held.
 keyStates[key] = true;

 // One-shot actions that should fire exactly once per press:
 switch (key)
 {
    // Task 4: Free the Spacebar for Jumping
    case ' ': myvirtualworld.kinger.jump(); break;

    // Task 4: Map Roll Skill
    case 'c':
    case 'C':
        myvirtualworld.kinger.animation.castRollSkill();
        break;

    // Task 5: Input Binding for Reload
    case 'r':
    case 'R':
        myvirtualworld.kinger.animation.castReload();
        break;

    // Task 4: Map Heal Skill
    case 'f':
    case 'F':
        myvirtualworld.kinger.animation.castHealSkill(myvirtualworld.kinger.currentHealth, myvirtualworld.kinger.maxHealth);
        break;

    case 27: exit(1); break; // ESC
 }
 glutPostRedisplay();
}

// --------------------------------------------------------------------------
// myKeyboardUpFunc — key UP: clear state.
// Registered with glutKeyboardUpFunc so the keyStates array always reflects
// which keys are physically held at the time of each frame update.
// --------------------------------------------------------------------------
void myKeyboardUpFunc(unsigned char key, int x, int y)
{
 keyStates[key] = false;
}

void mySpecialFunc(int key, int x, int y)
{
 switch (key)
 {
    // Arrow keys: keyboard fallback for camera look (mouse is primary).
    // UP / DOWN adjust vertical angle (pitch); LEFT / RIGHT adjust horizontal (yaw).
    case GLUT_KEY_UP   : cameraPitch -= CAMERA_KEY_TURN_INC;
                         if (cameraPitch < CAMERA_PITCH_MIN) cameraPitch = CAMERA_PITCH_MIN;
                         break;
    case GLUT_KEY_DOWN : cameraPitch += CAMERA_KEY_TURN_INC;
                         if (cameraPitch > CAMERA_PITCH_MAX) cameraPitch = CAMERA_PITCH_MAX;
                         break;
    case GLUT_KEY_LEFT : cameraYaw -= CAMERA_KEY_TURN_INC; break;
    case GLUT_KEY_RIGHT: cameraYaw += CAMERA_KEY_TURN_INC; break;

    case GLUT_KEY_HOME: myDataInit(); break;
 	case GLUT_KEY_F1  : setting.shadingMode = !setting.shadingMode;
                        if (setting.shadingMode)
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        else
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        break;
 	case GLUT_KEY_F2  : worldaxis.toggle(); break;
 	case GLUT_KEY_F3  : GLboolean lightingIsOn;
                        glGetBooleanv(GL_LIGHTING, &lightingIsOn);
                        if (lightingIsOn == GL_TRUE)
                            glDisable(GL_LIGHTING);
                        else
                            glEnable(GL_LIGHTING);
                        break;
 }
 glutPostRedisplay();
}

// --------------------------------------------------------------------------
// myPassiveMotionFunc — called by GLUT whenever the mouse moves without a
// button held. We warp the cursor back to the window centre every frame so
// it never hits the edge, then accumulate the delta into cameraYaw / Pitch.
// --------------------------------------------------------------------------
void myPassiveMotionFunc(int x, int y)
{
 int centerX = window.width  / 2;
 int centerY = window.height / 2;

 // Ignore the synthetic warp callback to prevent a feedback loop.
 if (x == centerX && y == centerY)
     return;

 int deltaX = x - centerX;
 int deltaY = y - centerY;

 // Horizontal mouse movement rotates the camera left/right (yaw).
 cameraYaw -= deltaX * MOUSE_SENSITIVITY;

 // Vertical mouse movement tilts the camera up/down (pitch).
 // In GLUT, deltaY > 0 means the mouse moved DOWN the screen.
 // Moving the mouse DOWN should lower the camera (reduce pitch),
 // so we SUBTRACT deltaY to match standard TPS "mouse-forward = look up" feel.
 cameraPitch += deltaY * MOUSE_SENSITIVITY;

 // Clamp pitch so the camera cannot flip past straight up or below ground.
 if (cameraPitch > CAMERA_PITCH_MAX) cameraPitch = CAMERA_PITCH_MAX;
 if (cameraPitch < CAMERA_PITCH_MIN) cameraPitch = CAMERA_PITCH_MIN;

 // Warp the system cursor back to the centre of the window.
 glutWarpPointer(centerX, centerY);

 glutPostRedisplay();
}

void myMouseFunc(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            {
                // Task 4: Map Mouse to Shoot
                // Fire immediately on click
                myvirtualworld.kinger.animation.castGunSkill();

                // Map Left Click to a special keyState index (1) for continuous auto-fire
                keyStates[1] = true;
            }
            if (state == GLUT_UP)
            {
                keyStates[1] = false;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            // Left click is currently unused in TPS mode,
            // but kept structurally intact.
            break;
    }
}

void myMotionFunc(int x, int y)
{
    // In TPS mode, clicking and dragging (MotionFunc) should do exactly
    // the same thing as moving the mouse without clicking (PassiveMotionFunc).
    // This allows the player to continue aiming while holding Right Click to shoot!
    myPassiveMotionFunc(x, y);
}

void myDataInit()
{
 window.title = "TCG6223 Computer Graphics";
 window.posX = 100;
 window.posY = 100;
 window.width  = 800;
 window.height = 500;

 world.rotateX  = 0.0;
 world.rotateY  = 0.0;
 world.rotateZ  = 0.0;
 world.posX     = 0.0;
 world.posY     = 0.0;
 world.posZ     = 0.0;
 world.scaleX   = 1.0;
 world.scaleY   = 1.0;
 world.scaleZ   = 1.0;

 viewer.eyeX    = 0.0;
 viewer.eyeY    = 0.0;
 viewer.eyeZ    = 40.0;
 viewer.centerX = 0.0;
 viewer.centerY = 0.0;
 viewer.centerZ = 0.0;
 viewer.upX     = 0.0;
 viewer.upY     = 1.0;
 viewer.upZ     = 0.0;
 viewer.zNear   = 0.1;
 viewer.zFar    = 2000.0;
 viewer.fieldOfView = 60.0;
 viewer.aspectRatio = static_cast<GLdouble> (window.width) / window.height;

 setting.posInc   = 1.0;
 setting.angleInc = 2.0;
 setting.mouseX   = 0;
 setting.mouseY   = 0;

 setting.mouseRightMode = false;
 setting.mouseLeftMode = false;

 setting.shadingMode = true;
}

void myViewingInit()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(viewer.fieldOfView,
                viewer.aspectRatio,
                viewer.zNear,
                viewer.zFar);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(viewer.eyeX,   viewer.eyeY,   viewer.eyeZ,
           viewer.centerX,viewer.centerY,viewer.centerZ,
           viewer.upX,    viewer.upY,    viewer.upZ );
}

void myLightingInit()
{
 static GLfloat  ambient[] = { 0.0f,  0.0f,  0.0f, 1.0f };
 static GLfloat  diffuse[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat  specref[] = { 1.0f,  1.0f,  1.0f, 1.0f };
 static GLfloat position[] = {10.0f, 10.0f, 10.0f, 1.0f };
  short shininess = 128;

 glDisable(GL_LIGHTING);
 glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
 glLightfv(GL_LIGHT0, GL_POSITION, position);
 glEnable(GL_LIGHT0);

 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 glEnable(GL_COLOR_MATERIAL);

 glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
 glMateriali(GL_FRONT, GL_SHININESS, shininess);

 glEnable(GL_NORMALIZE);
}

void myInit()
{
 myDataInit();

 glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
 glutInitWindowPosition(window.posX, window.posY); // Set top-left position
 glutInitWindowSize(window.width, window.height); //Set width and height
 glutCreateWindow(window.title.c_str());// Create display window

 glutDisplayFunc(myDisplayFunc);
 glutReshapeFunc(myReshapeFunc);
 glutKeyboardFunc(myKeyboardFunc);
 //glutKeyboardUpFunc(myKeyboardUpFunc);  // clears keyStates when keys are released
 glutSpecialFunc(mySpecialFunc);
 glutMotionFunc(myMotionFunc);
 glutMouseFunc(myMouseFunc);
 glutPassiveMotionFunc(myPassiveMotionFunc); // mouse-look without button
 glutSetCursor(GLUT_CURSOR_NONE);            // hide cursor for immersive look

 glPointSize(4.0);
 glEnable(GL_DEPTH_TEST);
 glDepthFunc(GL_LESS);
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 glFrontFace(GL_CCW);
 glShadeModel (GL_SMOOTH);
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
 glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

 glEnable(GL_CULL_FACE);

 myViewingInit();

 myLightingInit();

 myvirtualworld.init();
}

void myWelcome()
{
 cout << "*****************************************************************\n";
 cout << "*                   TCG6223 Computer Graphics                   *\n";
 cout << "*                  FIST, Multimedia University                  *\n";
 cout << "*****************************************************************\n";
 cout << "| TPS Controls:                                                 |\n";
 cout << "|   <w>,<s>             => move Kinger forward / backward       |\n";
 cout << "|   <a>,<d>             => strafe Kinger left / right           |\n";
 cout << "|   Mouse (move)        => rotate camera (yaw + pitch)          |\n";
 cout << "|   Arrow UP/DOWN       => camera pitch (keyboard fallback)     |\n";
 cout << "|   Arrow LEFT/RIGHT    => camera yaw   (keyboard fallback)     |\n";
 cout << "|   SPACE               => jump                                 |\n";
 cout << "|   HOME                => restore defaults                     |\n";
 cout << "|   ESC                 => exit                                 |\n";
 cout << "|                                                               |\n";
 cout << "|   F1  => toggle shading / wire-frame                          |\n";
 cout << "|   F2  => toggle axis rendering                                |\n";
 cout << "|   F3  => toggle lighting on / off                             |\n";
 cout << "*****************************************************************\n";
 cout << "|                      H A V E   F U N  !!!                    |\n";
 cout << "*****************************************************************\n";
}

//--------------------------------------------------------------------
int main(int argc, char **argv)
{
 glutInit(&argc, argv);

 myWelcome();

 myInit();

 glutMainLoop(); // Display everything and wait
}
//--------------------------------------------------------------------
