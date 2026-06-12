#include <windows.h>
#include <GL/gl.h>
#include "Environment.hpp"
#include "TextureLoader.hpp"

using namespace ProjectEnvironment;

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

    // Textures
    circusObject1Texture = 0;
    circusObject2Texture = 0;
}
bool Environment::loadTextures()
{
    circusObject1Texture = TextureLoader::loadTexture("Model\\Environment\\Textures\\CircusObject1.png");
    circusObject2Texture = TextureLoader::loadTexture("Model\\Environment\\Textures\\CircusObject2.png");

    return circusObject1Texture != 0 && circusObject2Texture != 0;
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

////////////////////////////////////Draw////////////////////////////////////
void Environment::drawSkyBox() const
{
    if (!skyBoxLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(180, 210, 255);

    //Size of the SkyBox
    glScalef(15.0f, 15.0f, 15.0f);

    skyBoxModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawGround() const
{
    if (!groundLoaded)
        return;

    glPushMatrix();

    // No texture for now
    glDisable(GL_TEXTURE_2D);

    // Ground color for testing
    glColor3ub(120, 170, 100);

    // If the ground does not appear, keep culling disabled.
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    //Transformation
    glTranslatef(0.0f, -18.7f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);

    groundModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawRoof() const
{
    if (!roofLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(120, 120, 180);

    //Size and Translate of the roof
    glTranslatef(0.0f, 18.7f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);

    roofModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawCastleWall() const
{
    if (!castleWallLoaded)
        return;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(150, 150, 150);

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

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawCube() const
{
    if (!cubeLoaded)
        return;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Cube left (Dark Orange)
    glPushMatrix();
    glColor3ub(220, 110, 60);
    glTranslatef(-90.0f, -18.7f, 100.0f);
    glScalef(8.0f, 8.0f, 8.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube left (Mint Green)
    glPushMatrix();
    glColor3ub(168, 230, 207);
    glTranslatef(-180.0f, -18.7f, 40.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube right (Light Blue)
    glPushMatrix();
    glColor3ub(80, 190, 220);
    glTranslatef(90.0f, -18.7f, -130.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube right (Coral Pink)
    glPushMatrix();
    glColor3ub(255, 111, 97);
    glTranslatef(20.0f, -18.7f, -210.0f);
    glScalef(17.0f, 17.0f, 17.0f);
    cubeModel.draw();
    glPopMatrix();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawCubeGrouped() const
{
    if (!cubeGroupedLoaded)
        return;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Purple grouped block near center but slightly behind
    glPushMatrix();
    glColor3ub(170, 100, 220);
    glTranslatef(220.0f, -18.7f, 150.0f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    cubeGroupedModel.draw();
    glPopMatrix();

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
    glTranslatef(-42.0f, -18.7f, 25.0f);
    glScalef(12.0f, 12.0f, 12.0f);
    irregularCubeModel.draw();
    glPopMatrix();

    // Abstract prop 2 - CircusObject2 texture
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, circusObject2Texture);
    glTranslatef(-55.0f, -18.7f, 46.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(9.0f, 9.0f, 9.0f);
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

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(210, 200, 150);

    // Front-left pillar (Edited)
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

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}

void Environment::drawSphere() const
{
    if (!sphereLoaded)
        return;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // Main yellow floating ball above center
    glPushMatrix();
    glColor3ub(255, 220, 120);
    glTranslatef(-10.0f, 50.0f, 10.0f);
    glScalef(9.0f, 9.0f, 9.0f);
    sphereModel.draw();
    glPopMatrix();

    // Red floating ball left-back
    glPushMatrix();
    glColor3ub(240, 80, 80);
    glTranslatef(-60.0f, 20.0f, -45.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    // Blue floating ball front-left
    glPushMatrix();
    glColor3ub(80, 160, 255);
    glTranslatef(-90.0f, 30.0f, 90.0f);
    glScalef(20.0f, 20.0f, 20.0f);
    sphereModel.draw();
    glPopMatrix();

    // Purple floating ball right-back
    glPushMatrix();
    glColor3ub(180, 100, 230);
    glTranslatef(75.0f, 35.0f, -60.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    sphereModel.draw();
    glPopMatrix();

    // Orange floating ball right-front
    glPushMatrix();
    glColor3ub(255, 127, 0);
    glTranslatef(85.0f, 25.0f, 100.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
}


/////////////////////////////////////Main draw function//////////////////////////
void Environment::draw() const
{
    drawSkyBox();
    drawGround();
    drawRoof();

    drawCastleWall();
    drawCube();
    drawCubeGrouped();
    drawIrregularCube();
    drawPillar();
    drawSphere();
}
