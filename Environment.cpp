#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include "Environment.hpp"
#include "TextureLoader.hpp"

using namespace ProjectEnvironment;

// =======================Local environment object lighting====================== //
static void enableLocalEnvironmentLight(
    GLenum lightID,
    GLfloat x, GLfloat y, GLfloat z,
    GLfloat r, GLfloat g, GLfloat b,
    GLfloat linearAttenuation,
    GLfloat quadraticAttenuation
)
{
    glEnable(lightID);

    // 1.0f means point light, not directional light
    GLfloat lightPosition[] = { x, y, z, 1.0f };

    // Keep ambient very low because we only want local highlight
    GLfloat lightAmbient[]  = { 0.00f, 0.00f, 0.00f, 1.0f };
    GLfloat lightDiffuse[]  = { r, g, b, 1.0f };
    GLfloat lightSpecular[] = { r * 0.45f, g * 0.45f, b * 0.45f, 1.0f };

    glLightfv(lightID, GL_POSITION, lightPosition);
    glLightfv(lightID, GL_AMBIENT,  lightAmbient);
    glLightfv(lightID, GL_DIFFUSE,  lightDiffuse);
    glLightfv(lightID, GL_SPECULAR, lightSpecular);

    // Control how far the local light reaches
    glLightf(lightID, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(lightID, GL_LINEAR_ATTENUATION, linearAttenuation);
    glLightf(lightID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
}

static void disableLocalEnvironmentLight(GLenum lightID)
{
    glDisable(lightID);
}
// ============================================================================ //

// Glitch Animation Helper Function
static void drawGlitchPanel(float width, float height, float uShift)
{
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f + uShift, 0.0f);
        glVertex3f(-width, -height, 0.0f);

        glTexCoord2f(1.0f + uShift, 0.0f);
        glVertex3f( width, -height, 0.0f);

        glTexCoord2f(1.0f + uShift, 1.0f);
        glVertex3f( width,  height, 0.0f);

        glTexCoord2f(0.0f + uShift, 1.0f);
        glVertex3f(-width,  height, 0.0f);
    glEnd();
}

Environment::Environment()
{
    // Objects
    skyBoxLoaded = false;
    groundLoaded = false;
    roofLoaded = false;
    castleWallLoaded = false;
    cubeLoaded = false;
    cubeGroupedLoaded = false;
    irregularCubeLoaded = false;
    pillarLoaded = false;
    sphereLoaded = false;

    // Animation
    animationTime = 0.0f;

    // Textures
    skyBoxTexture = 0;
    groundTexture = 0;
    roofTexture = 0;
    castleWallTexture = 0;
    cube1Texture = 0;
    cube2Texture = 0;
    cubeGroupedTexture = 0;
    circusObject1Texture = 0;
    circusObject2Texture = 0;
    pillarTexture = 0;
    skyTexture = 0;

    // Animation Textures
    glitchTexture = 0;

}
bool Environment::loadTextures()
{
    skyBoxTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\CircusStrip.jpg"
    );

    groundTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\FloorStrip.png"
    );

    roofTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\Circus.jpg"
    );

    castleWallTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\CastleWall.jpg"
    );

    cube1Texture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\Cube1.png"
    );

    cube2Texture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\Cube2.png"
    );

    cubeGroupedTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\CubeGrouped.png"
    );

    circusObject1Texture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\CircusObject1.png"
    );

    circusObject2Texture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\CircusObject2.png"
    );

    pillarTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\Pillar.png"
    );

    skyTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\Sky.jpg"
    );

    // Animation Texture
    glitchTexture = TextureLoader::loadTexture(
        "Model\\Environment\\Textures\\GlitchEffect.png"
    );

    return skyBoxTexture != 0 &&
           groundTexture != 0 &&
           roofTexture != 0 &&
           castleWallTexture != 0 &&
           cube1Texture != 0 &&
           cube2Texture != 0 &&
           cubeGroupedTexture != 0 &&
           circusObject1Texture != 0 &&
           circusObject2Texture != 0 &&
           pillarTexture != 0 &&
           skyTexture != 0 &&
           glitchTexture != 0;
}

bool Environment::loadSkyBox(const std::string& filePath)
{
    skyBoxLoaded = skyBoxModel.loadFromObjText(filePath);
    return skyBoxLoaded;
}

bool Environment::loadGround(const std::string& filePath)
{
    groundLoaded = groundModel.loadFromObjText(filePath);
    return groundLoaded;
}

bool Environment::loadCastleWall(const std::string& filePath)
{
    castleWallLoaded = castleWallModel.loadFromObjText(filePath);
    return castleWallLoaded;
}

bool Environment::loadCube(const std::string& filePath)
{
    cubeLoaded = cubeModel.loadFromObjText(filePath);
    return cubeLoaded;
}

bool Environment::loadCubeGrouped(const std::string& filePath)
{
    cubeGroupedLoaded = cubeGroupedModel.loadFromObjText(filePath);
    return cubeGroupedLoaded;
}

bool Environment::loadIrregularCube(const std::string& filePath)
{
    irregularCubeLoaded = irregularCubeModel.loadFromObjText(filePath);
    return irregularCubeLoaded;
}

bool Environment::loadPillar(const std::string& filePath)
{
    pillarLoaded = pillarModel.loadFromObjText(filePath);
    return pillarLoaded;
}

bool Environment::loadRoof(const std::string& filePath)
{
    roofLoaded = roofModel.loadFromObjText(filePath);
    return roofLoaded;
}

bool Environment::loadSphere(const std::string& filePath)
{
    sphereLoaded = sphereModel.loadFromObjText(filePath);
    return sphereLoaded;
}

//////////////////////////////////Animation/////////////////////////////////
void Environment::tickTime() //Time-Based
{
    static int previousTime = -1;

    int currentTime = glutGet(GLUT_ELAPSED_TIME);

    if (previousTime < 0)
    {
        previousTime = currentTime;
        return;
    }

    float deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;

    // Prevent a sudden big jump if the window freezes or pauses
    if (deltaTime > 0.1f)
    {
        deltaTime = 0.1f;
    }

    // Adjust this value if the animation is too slow or too fast
    const float animationSpeed = 1.0f;

    animationTime += deltaTime * animationSpeed;

    if (animationTime > 1000.0f)
    {
        animationTime = 0.0f;
    }
}

////////////////////////////////////Draw////////////////////////////////////
void Environment::drawSkyBox() const
{
    if (!skyBoxLoaded)
        return;

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyBoxTexture);

    // Usually better to disable lighting for background
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Darken the whole background texture here
    glColor3ub(180, 180, 180);

    // Make sure texture color is multiplied by glColor
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Size of SkyBox
    glScalef(15.0f, 15.0f, 15.0f);

    skyBoxModel.draw();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void Environment::drawGround() const
{
    if (!groundLoaded)
        return;

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, groundTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture color is not tinted
    glColor3ub(255, 255, 255);

    // Transformation
    glTranslatef(0.0f, -18.7f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);

    groundModel.draw();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawRoof() const
{
    if (!roofLoaded)
        return;

    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roofTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // White color so the roof texture appears correctly
    glColor3ub(255, 255, 255);

    //////////////////////////////Animation////////////////////////////
    // Slow roof rotation
    float roofRotateAngle = animationTime * 8.0f;
    ////////////////////////////////////////////////////////////////////

    // Size and translate of the roof
    glTranslatef(0.0f, 18.7f, 0.0f);
    // Rotate around Y-axis
    glRotatef(roofRotateAngle, 0.0f, 1.0f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);

    roofModel.draw();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawCastleWall() const
{
    if (!castleWallLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, castleWallTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture color is not tinted
    glColor3ub(255, 255, 255);

    // Back wall left
    glPushMatrix();
    glTranslatef(-66.5f, -18.7f, -66.5f);
    glScalef(10.0f, 15.0f, 10.0f);
    castleWallModel.draw();
    glPopMatrix();

    // Front wall left
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(-66.5f, -18.7f, -66.5f);
    glScalef(10.0f, 15.0f, 10.0f);
    castleWallModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawCube() const
{
    if (!cubeLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture color is not tinted
    glColor3ub(255, 255, 255);

    //////////////////////////////Animation////////////////////////////
    // Left-right movement
    float moveX1 = sin(animationTime) * 20.0f;
    // Forward-backward movement
    float moveZ2 = cos(animationTime * 0.8f) * 25.0f;
    // Circular movement
    float circleX = sin(animationTime * 0.7f) * 25.0f;
    float circleZ = cos(animationTime * 0.7f) * 25.0f;
    // Slower left-right movement
    float moveX4 = sin(animationTime * 0.5f) * 30.0f;
    // Small rotation for all cubes
    float rotateAngle = animationTime * 25.0f;
    ////////////////////////////////////////////////////////////////////

    // =====================================================
    // Cube left 1 - Cube1.png
    // Moves left and right
    // =====================================================
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube1Texture);

    glTranslatef(-90.0f + moveX1, -18.7f, 100.0f);
    glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    cubeModel.draw();
    glPopMatrix();
    // =====================================================

    // =====================================================
    // Cube left 2 - Cube2.png
    // Moves forward and backward
    // =====================================================
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube2Texture);

    glTranslatef(-180.0f, -18.7f, 40.0f + moveZ2);
    glRotatef(-rotateAngle * 0.8f, 0.0f, 1.0f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);

    cubeModel.draw();
    glPopMatrix();
    // =====================================================

    // =====================================================
    // Cube right 1 - Cube2.png
    // Circular movement
    // =====================================================
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube2Texture);

    glTranslatef(90.0f + circleX, -18.7f, -130.0f + circleZ);
    glRotatef(rotateAngle * 1.2f, 0.0f, 1.0f, 0.0f);
    glScalef(10.0f, 10.0f, 10.0f);

    cubeModel.draw();
    glPopMatrix();
    // =====================================================

    // =====================================================
    // Cube right 2 - Cube1.png again
    // Slower left and right movement
    // =====================================================
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube1Texture);

    glTranslatef(20.0f + moveX4, -18.7f, -210.0f);
    glRotatef(-rotateAngle * 0.6f, 0.0f, 1.0f, 0.0f);
    glScalef(17.0f, 17.0f, 17.0f);

    cubeModel.draw();
    glPopMatrix();
    // =====================================================

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawCubeGrouped() const
{
    if (!cubeGroupedLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cubeGroupedTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture color is not tinted
    glColor3ub(255, 255, 255);

    // Grouped block near center but slightly behind
    glPushMatrix();
    glTranslatef(220.0f, -18.7f, 150.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    cubeGroupedModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawIrregularCube() const
{
    if (!irregularCubeLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white color so the texture color appears correctly
    glColor3ub(255, 255, 255);

    // Abstract prop 1 - CircusObject1 texture
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, circusObject1Texture);
    glTranslatef(-42.0f, -18.7f, 0.0f);
    glScalef(18.0f, 18.0f, 18.0f);
    irregularCubeModel.draw();
    glPopMatrix();

    // Abstract prop 2 - CircusObject2 texture
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, circusObject2Texture);
    glTranslatef(-55.0f, -18.7f, 22.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(12.0f, 12.0f, 12.0f);
    irregularCubeModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawPillar() const
{
    if (!pillarLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pillarTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture is not tinted
    glColor3ub(255, 255, 255);

    // Front-left pillar
    glPushMatrix();
    glTranslatef(-150.0f, -18.7f, 27.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    pillarModel.draw();
    glPopMatrix();

    // Back-right pillar
    glPushMatrix();
    glTranslatef(150.0f, -18.7f, -270.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    pillarModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawSphere() const
{
    if (!sphereLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Use white so the texture color appears correctly
    glColor3ub(255, 255, 255);

    //////////////////////////////Animation////////////////////////////
    // Different floating values so spheres do not move together
    float floatY1 = sin(animationTime) * 5.0f;
    float floatY2 = sin(animationTime + 1.2f) * 4.0f;
    float floatY3 = sin(animationTime + 2.4f) * 6.0f;
    float floatY4 = sin(animationTime + 3.6f) * 5.0f;
    float floatY5 = sin(animationTime + 4.8f) * 4.0f;
    // Rotation value
    float rotateAngle = animationTime * 30.0f;
    ////////////////////////////////////////////////////////////////////

    // Main floating ball above center
    glPushMatrix();
    glTranslatef(-10.0f, 50.0f + floatY1, 10.0f);
    glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
    glScalef(9.0f, 9.0f, 9.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball left-back
    glPushMatrix();
    glTranslatef(-60.0f, 20.0f + floatY2, -45.0f);
    glRotatef(rotateAngle * 0.8f, 0.0f, 1.0f, 0.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball front-left
    glPushMatrix();
    glTranslatef(-90.0f, 30.0f + floatY3, 90.0f);
    glRotatef(rotateAngle * 0.6f, 0.0f, 1.0f, 0.0f);
    glScalef(20.0f, 20.0f, 20.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball right-back
    glPushMatrix();
    glTranslatef(75.0f, 35.0f + floatY4, -60.0f);
    glRotatef(rotateAngle * 1.1f, 0.0f, 1.0f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball right-front
    glPushMatrix();
    glTranslatef(85.0f, 25.0f + floatY5, 100.0f);
    glRotatef(rotateAngle * 0.9f, 0.0f, 1.0f, 0.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawDigitalEffect() const
{
    // Save current lighting state
    GLboolean lightingWasOn;
    glGetBooleanv(GL_LIGHTING, &lightingWasOn);

    // Glitch effect should look like glowing screen distortion
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glitchTexture);

    // Additive blend makes bright glitch parts glow
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Do not let transparent glitch panels block the scene
    glDepthMask(GL_FALSE);

    // Make texture react to alpha/color
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // =====================================================
    // Wider floating glitch panels across the environment
    // =====================================================
    for (int i = 0; i < 9; i++)
    {
        float x = -120.0f + (i % 3) * 120.0f;
        float z = -120.0f + (i / 3) * 100.0f;

        float y = 35.0f + (i % 3) * 12.0f
                + sin(animationTime * 1.5f + i) * 4.0f;

        float flicker = 0.12f + 0.35f * fabs(sin(animationTime * 7.0f + i));
        float uShift = sin(animationTime * 3.0f + i) * 0.20f;

        float width = 22.0f + (i % 2) * 12.0f;
        float height = 8.0f + (i % 3) * 4.0f;

        glPushMatrix();

        glColor4f(1.0f, 1.0f, 1.0f, flicker);

        glTranslatef(x, y, z);

        // Make each panel face a slightly different direction
        glRotatef(i * 35.0f + sin(animationTime + i) * 10.0f,
                  0.0f, 1.0f, 0.0f);

        drawGlitchPanel(width, height, uShift);

        glPopMatrix();
    }

    // =====================================================
    // More thin horizontal glitch scan lines
    // =====================================================
    glDisable(GL_TEXTURE_2D);
    glLineWidth(2.0f);

    for (int j = 0; j < 18; j++)
    {
        float x = -140.0f + (j % 6) * 55.0f;
        float z = -150.0f + (j / 6) * 100.0f;
        float y = 25.0f + (j % 4) * 14.0f;

        float moveX = sin(animationTime * 2.0f + j) * 8.0f;
        float blink = fabs(sin(animationTime * 9.0f + j));

        // Cyan-blue digital line
        glColor4f(0.0f, 0.9f, 1.0f, blink * 0.55f);

        glBegin(GL_LINES);
            glVertex3f(x + moveX, y, z);
            glVertex3f(x + moveX + 35.0f, y + 2.0f, z);
        glEnd();
    }

    glLineWidth(1.0f);

    // Restore states
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if (lightingWasOn)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);
}

/////////////////////////////////////Main draw function//////////////////////////
void Environment::draw() const
{
    // Background first
    drawSkyBox();

    // =====================================================
    // Ground and roof: dim overhead stage light
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        0.0f, 80.0f, 0.0f,        // position
        0.35f, 0.35f, 0.45f,      // cool dim white-blue color
        0.002f,
        0.00001f
    );

    drawGround();
    drawRoof();

    disableLocalEnvironmentLight(GL_LIGHT1);


    // =====================================================
    // Castle walls: cold blue side light
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        -120.0f, 60.0f, -80.0f,   // position near left/back wall
        0.35f, 0.45f, 0.85f,      // blue lighting
        0.003f,
        0.00002f
    );

    drawCastleWall();

    disableLocalEnvironmentLight(GL_LIGHT1);


    // =====================================================
    // Cubes: red circus/battle light
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        70.0f, 60.0f, -80.0f,     // position near cube area
        0.90f, 0.20f, 0.20f,      // red light
        0.003f,
        0.00002f
    );

    drawCube();
    drawCubeGrouped();

    disableLocalEnvironmentLight(GL_LIGHT1);


    // =====================================================
    // Irregular circus objects: purple/magenta light
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        -50.0f, 45.0f, 40.0f,     // position near irregular cubes
        0.80f, 0.20f, 0.90f,      // purple light
        0.004f,
        0.00003f
    );

    drawIrregularCube();

    disableLocalEnvironmentLight(GL_LIGHT1);


    // =====================================================
    // Pillars: soft warm spotlight
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        100.0f, 90.0f, -170.0f,   // position near pillars
        0.85f, 0.75f, 0.55f,      // warm beige light
        0.003f,
        0.00002f
    );

    drawPillar();

    disableLocalEnvironmentLight(GL_LIGHT1);


    // =====================================================
    // Floating spheres: blue sky glow
    // =====================================================
    enableLocalEnvironmentLight(
        GL_LIGHT1,
        0.0f, 90.0f, 20.0f,       // position near middle/top
        0.25f, 0.55f, 1.00f,      // blue light
        0.003f,
        0.00002f
    );

    drawSphere();

    disableLocalEnvironmentLight(GL_LIGHT1);

    // Digital zap-zap circus effect
    drawDigitalEffect();
}
