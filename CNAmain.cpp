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

static const float CAMERA_DISTANCE           = 60.0f;  // units BEHIND Kinger (must be positive)
static const float CAMERA_BASE_HEIGHT        = 30.0f;  // extra height above the target point
static const float CAMERA_SHOULDER_OFFSET    = 18.0f;  // Over-the-shoulder offset to the right
static const float CAMERA_TARGET_HEIGHT_OFFSET = 5.0f; // Kinger's visual centre above Y=0 (chest level)
static const float CAMERA_PITCH_MAX          =  1.2f;  // radians (~69 deg, camera high up)
static const float CAMERA_PITCH_MIN          = -1.2f;  // radians (steep upward aiming allowed)
static const float MOUSE_SENSITIVITY         =  0.003f;
static const float KINGER_MOVE_SPEED         =  1.0f;  // world units per key press
static const float CAMERA_KEY_TURN_INC       =  0.04f; // radians per arrow key

float boundaryScale = 14.5f; // Global scale for the logical boundary of Kinger and camera

float cameraYaw   = 0.0f;  // horizontal camera angle, radians
float cameraPitch = 0.4f;  // vertical camera angle, radians 


float cameraTrackY = -18.7f;
const float CAMERA_FOLLOW_SPEED = 10.0f;

bool keyStates[256] = {false};

// Global variables tracking camera position and look direction
float currentCameraEyeX = 0.0f;
float currentCameraEyeY = 0.0f;
float currentCameraEyeZ = 0.0f;

float currentCameraDirX = 0.0f;
float currentCameraDirY = 0.0f;
float currentCameraDirZ = 0.0f;

bool showHitboxes = false; // Toggle to render green hitboxes around active Gloinks
bool showDebugMenu = false; // Toggle to render the debug context menu on the right

void drawCrosshair()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window.width, 0, window.height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    float cx = window.width / 2.0f;
    float cy = window.height / 2.0f;
    float size = 10.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex2f(cx - size, cy);
        glVertex2f(cx + size, cy);
        glVertex2f(cx, cy - size);
        glVertex2f(cx, cy + size);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

static void drawString(void* font, const char* str, float x, float y)
{
    glRasterPos2f(x, y);
    while (*str)
    {
        glutBitmapCharacter(font, *str);
        str++;
    }
}

void drawDebugMenu()
{
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window.width, 0, window.height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    if (showDebugMenu)
    {
        // Define panel boundaries
        float width = 290.0f;
        float height = 240.0f;
        float padding = 20.0f;
        float right = window.width - padding;
        float left = right - width;
        float top = window.height - padding;
        float bottom = top - height;

        // Draw semi-transparent background (Valve Dark theme)
        glColor4f(0.08f, 0.08f, 0.12f, 0.85f);
        glBegin(GL_QUADS);
            glVertex2f(left, bottom);
            glVertex2f(right, bottom);
            glVertex2f(right, top);
            glVertex2f(left, top);
        glEnd();

        // Draw border (Source Console light border)
        glLineWidth(2.0f);
        glColor4f(0.35f, 0.5f, 0.75f, 0.7f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(left, bottom);
            glVertex2f(right, bottom);
            glVertex2f(right, top);
            glVertex2f(left, top);
        glEnd();

        // Text color for options (Source Console Light Green/Yellow)
        glColor4f(0.0f, 0.9f, 0.5f, 1.0f);
        drawString(GLUT_BITMAP_HELVETICA_18, "--- DEBUG MENU ---", left + 20.0f, top - 30.0f);

        glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
        drawString(GLUT_BITMAP_HELVETICA_18, "1. Start Game", left + 25.0f, top - 70.0f);
        drawString(GLUT_BITMAP_HELVETICA_18, "2. Reset Game", left + 25.0f, top - 105.0f);
        drawString(GLUT_BITMAP_HELVETICA_18, "3. Debug Environment", left + 25.0f, top - 140.0f);
        
        glColor4f(0.7f, 0.7f, 0.8f, 1.0f);
        drawString(GLUT_BITMAP_HELVETICA_18, "0. Exit Menu", left + 25.0f, top - 180.0f);

        // Status lines at the bottom of panel
        glColor4f(0.5f, 0.6f, 0.7f, 1.0f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
            glVertex2f(left + 15.0f, bottom + 40.0f);
            glVertex2f(right - 15.0f, bottom + 40.0f);
        glEnd();

        glColor4f(0.8f, 0.8f, 0.8f, 0.9f);
        if (myvirtualworld.isDebugMode)
        {
            drawString(GLUT_BITMAP_HELVETICA_12, "Mode: DEBUG ENVIRONMENT (Keys 1-0 active)", left + 20.0f, bottom + 15.0f);
        }
        else
        {
            drawString(GLUT_BITMAP_HELVETICA_12, "Mode: ACTIVE GAMEPLAY (Keys 1-0 disabled)", left + 20.0f, bottom + 15.0f);
        }
    }
    else
    {
        // Menu is closed, render a subtle hint on the right
        float width = 170.0f;
        float height = 35.0f;
        float padding = 20.0f;
        float right = window.width - padding;
        float left = right - width;
        float top = window.height - padding;
        float bottom = top - height;

        // Semi-transparent tiny hint bg
        glColor4f(0.08f, 0.08f, 0.12f, 0.6f);
        glBegin(GL_QUADS);
            glVertex2f(left, bottom);
            glVertex2f(right, bottom);
            glVertex2f(right, top);
            glVertex2f(left, top);
        glEnd();

        glColor4f(0.35f, 0.5f, 0.75f, 0.5f);
        glBegin(GL_LINE_LOOP);
            glVertex2f(left, bottom);
            glVertex2f(right, bottom);
            glVertex2f(right, top);
            glVertex2f(left, top);
        glEnd();

        glColor4f(0.0f, 0.9f, 0.5f, 0.9f);
        drawString(GLUT_BITMAP_HELVETICA_12, "[z] Open Debug Menu", left + 15.0f, bottom + 12.0f);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void updateKeyStatesFromWindows()
{
    keyStates['w'] = keyStates['W'] = (GetAsyncKeyState('W') & 0x8000) != 0;
    keyStates['a'] = keyStates['A'] = (GetAsyncKeyState('A') & 0x8000) != 0;
    keyStates['s'] = keyStates['S'] = (GetAsyncKeyState('S') & 0x8000) != 0;
    keyStates['d'] = keyStates['D'] = (GetAsyncKeyState('D') & 0x8000) != 0;

    keyStates[1] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
}

void myDisplayFunc(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 // Get player position and scale for camera calculations
 float& kX = myvirtualworld.kinger.posX;
 float& kZ = myvirtualworld.kinger.posZ;
 const float kScale = myvirtualworld.kinger.uniformScale;

 static int lastTime = glutGet(GLUT_ELAPSED_TIME);
 int currentTime = glutGet(GLUT_ELAPSED_TIME);
 float deltaTime = (currentTime - lastTime) / 1000.0f;
 if (deltaTime > 0.1f) deltaTime = 0.1f;
 lastTime = currentTime;
 
 // Smoothly follow the player's Y position
 cameraTrackY += (myvirtualworld.kinger.posY - cameraTrackY) * CAMERA_FOLLOW_SPEED * deltaTime;

 // The camera target's Y position. It's based on the model's origin (18.7f above its base)
 // plus a shoulder offset. Both are now correctly scaled with the model.
 const float baseTargetY = cameraTrackY + (18.7f + CAMERA_TARGET_HEIGHT_OFFSET) * kScale;

 float rightX = std::cos(cameraYaw);
 float rightZ = -std::sin(cameraYaw);

 // The camera's look-at point, offset for an over-the-shoulder view.
 // This offset is now scaled to feel consistent regardless of player size.
  float targetX = kX + rightX * (CAMERA_SHOULDER_OFFSET * kScale);
  float targetZ = kZ + rightZ * (CAMERA_SHOULDER_OFFSET * kScale);

  float cosPitch = std::cos(cameraPitch);
  float sinPitch = std::sin(cameraPitch);

  // The camera's ideal displacement vector from target
  float vx = std::sin(cameraYaw) * cosPitch * (CAMERA_DISTANCE * kScale);
  float vy = (CAMERA_BASE_HEIGHT * kScale) + sinPitch * (CAMERA_DISTANCE * kScale);
  float vz = std::cos(cameraYaw) * cosPitch * (CAMERA_DISTANCE * kScale);

  // Retrieve skybox bounds for camera collision limit
  Vec3 skyMin, skyMax;
  myvirtualworld.environment.getSkyBoxBounds(skyMin, skyMax);
  float limitMinX = skyMin.x * boundaryScale;
  float limitMaxX = skyMax.x * boundaryScale;
  float limitMinZ = skyMin.z * boundaryScale;
  float limitMaxZ = skyMax.z * boundaryScale;
  if (limitMinX > limitMaxX) std::swap(limitMinX, limitMaxX);
  if (limitMinZ > limitMaxZ) std::swap(limitMinZ, limitMaxZ);

  float margin = 5.0f;
  float t = 1.0f;

  if (vx > 0.001f)
  {
      float tMaxX = ((limitMaxX - margin) - targetX) / vx;
      if (tMaxX < t) t = tMaxX;
  }
  else if (vx < -0.001f)
  {
      float tMinX = ((limitMinX + margin) - targetX) / vx;
      if (tMinX < t) t = tMinX;
  }

  if (vz > 0.001f)
  {
      float tMaxZ = ((limitMaxZ - margin) - targetZ) / vz;
      if (tMaxZ < t) t = tMaxZ;
  }
  else if (vz < -0.001f)
  {
      float tMinZ = ((limitMinZ + margin) - targetZ) / vz;
      if (tMinZ < t) t = tMinZ;
  }

  if (t < 0.1f) t = 0.1f;

  // The camera's final world position, zoomed in if close to the circus walls
  float eyeX = targetX + t * vx;
  float eyeY = baseTargetY + t * vy;
  float eyeZ = targetZ + t * vz;

 const float GROUND_LEVEL = myvirtualworld.kinger.posY;
 if (eyeY < GROUND_LEVEL + 1.0f)
 {
     eyeY = GROUND_LEVEL + 1.0f;
 }

 // Record camera position to global variables
 currentCameraEyeX = eyeX;
 currentCameraEyeY = eyeY;
 currentCameraEyeZ = eyeZ;

 // Calculate and record camera look direction to global variables
 float dx = targetX - eyeX;
 float dy = baseTargetY - eyeY;
 float dz = targetZ - eyeZ;
 float len = std::sqrt(dx * dx + dy * dy + dz * dz);
 if (len > 0.0f)
 {
     currentCameraDirX = dx / len;
     currentCameraDirY = dy / len;
     currentCameraDirZ = dz / len;
 }
 else
 {
     currentCameraDirX = -std::sin(cameraYaw) * std::cos(cameraPitch);
     currentCameraDirY = -std::sin(cameraPitch);
     currentCameraDirZ = -std::cos(cameraYaw) * std::cos(cameraPitch);
 }

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(
     eyeX,    eyeY,        eyeZ,
     targetX, baseTargetY, targetZ, 
     0.0f,    1.0f,        0.0f 
 );

 glPushMatrix();

    worldaxis.draw();

    myvirtualworld.draw();

 glPopMatrix();

 // Draw 2D UI elements last
 drawCrosshair();
 drawDebugMenu();

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

void myKeyboardFunc(unsigned char key, int x, int y)
{
 // Toggle debug menu with 'z' or 'Z'
 if (key == 'z' || key == 'Z')
 {
     showDebugMenu = !showDebugMenu;
     glutPostRedisplay();
     return;
 }

 // If debug menu is open, handle navigation and intercept menu actions
 if (showDebugMenu)
 {
     switch (key)
     {
         case '1':
             myvirtualworld.startGame();
             showDebugMenu = false;
             break;
         case '2':
             myvirtualworld.resetGame();
             showDebugMenu = false;
             break;
         case '3':
             myvirtualworld.debugEnvironment();
             showDebugMenu = false;
             break;
         case '0':
             showDebugMenu = false;
             break;
     }
     glutPostRedisplay();
     return;
 }

 keyStates[key] = true;

 switch (key)
 {
    // Jumping
    case ' ': myvirtualworld.kinger.jump(); break;

    // Roll Skill
    case 'c':
    case 'C':
        myvirtualworld.kinger.animation.castRollSkill(myvirtualworld.kinger.isGrounded);
        break;

    // Reload
    case 'r':
    case 'R':
        myvirtualworld.kinger.animation.castReload();
        break;

    // Toggle Hitbox Visibility
    case 'b':
    case 'B':
        showHitboxes = !showHitboxes;
        break;

    // Heal Skill
    case 'f':
    case 'F':
        myvirtualworld.kinger.animation.castHealSkill(myvirtualworld.kinger.currentHealth, myvirtualworld.kinger.maxHealth);
        break;

    // Test Damage/Hurt Visual Effect
    case 'h':
    case 'H':
        myvirtualworld.kinger.takeDamage(10);
        break;

    // Test Death Visual Effect
    case 'k':
    case 'K':
        myvirtualworld.kinger.takeDamage(100);
        break;

    // Test Caine Hurt Visual Effect
    case 'j':
    case 'J':
        myvirtualworld.caine.triggerHurt();
        break;

    // Caine Hand Animation Trigger & Gloink Hurt Triggers (Only active in Debug Environment)
    case '1':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(0);
        break;
    case '2':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(1);
        break;
    case '3':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(2);
        break;
    case '4':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(3);
        break;
    case '5':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(4);
        break;
    case '6':
        if (myvirtualworld.isDebugMode) myvirtualworld.gloinks.hurtGloink(5);
        break;
    case '7':
        if (myvirtualworld.isDebugMode) myvirtualworld.caine.animation.triggerShootingState();
        break;
    case '8':
        if (myvirtualworld.isDebugMode) myvirtualworld.caine.animation.toggleLayDown();
        break;
    case '9':
        if (myvirtualworld.isDebugMode) myvirtualworld.caine.animation.toggleLeanForward();
        break;
    case '0':
        if (myvirtualworld.isDebugMode) myvirtualworld.caine.triggerDeath();
        break;

    case 27: exit(1); break; // ESC
 }
 glutPostRedisplay();
}

void myKeyboardUpFunc(unsigned char key, int x, int y)
{
 keyStates[key] = false;
}

void mySpecialFunc(int key, int x, int y)
{
 switch (key)
 {
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

void myPassiveMotionFunc(int x, int y)
{
 int centerX = window.width  / 2;
 int centerY = window.height / 2;

 if (x == centerX && y == centerY)
     return;

 int deltaX = x - centerX;
 int deltaY = y - centerY;

 cameraYaw -= deltaX * MOUSE_SENSITIVITY;

 cameraPitch += deltaY * MOUSE_SENSITIVITY;

 if (cameraPitch > CAMERA_PITCH_MAX) cameraPitch = CAMERA_PITCH_MAX;
 if (cameraPitch < CAMERA_PITCH_MIN) cameraPitch = CAMERA_PITCH_MIN;

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
                myvirtualworld.kinger.animation.castGunSkill();

                keyStates[1] = true;
            }
            if (state == GLUT_UP)
            {
                keyStates[1] = false;
            }
            break;

        case GLUT_RIGHT_BUTTON:
            break;
    }
}

void myMotionFunc(int x, int y)
{
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
 viewer.zFar    = 10000.0;
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
 cout << "|   <f>                 => cast heal skill                      |\n";
 cout << "|   <c>                 => roll                                 |\n";
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
 cout << "|   <b> => toggle hitbox outlines on / off                      |\n";
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
