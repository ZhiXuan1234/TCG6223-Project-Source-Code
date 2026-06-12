#include <windows.h>
#include <GL/gl.h>
#include "Environment.hpp"

using namespace ProjectEnvironment;

Environment::Environment()
{
    skyBoxLoaded = false;
    groundLoaded = false;

    castleWallLoaded = false;
    cubeLoaded = false;
    cubeGroupedLoaded = false;
    irregularCubeLoaded = false;
    pillarLoaded = false;
    roofLoaded = false;
    sphereLoaded = false;
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

void Environment::drawCastleWall() const
{
    if (!castleWallLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(160, 160, 160);

    // Move slightly to the back-left area
    glTranslatef(0.0f, -18.7f, 0.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    castleWallModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawCube() const
{
    if (!cubeLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(200, 120, 80);

    // Move cube forward-right for testing
    glTranslatef(15.0f, -18.7f, 0.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    cubeModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawCubeGrouped() const
{
    if (!cubeGroupedLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(170, 120, 200);

    // Move grouped cube to another side for testing
    glTranslatef(-15.0f, -18.7f, 0.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    cubeGroupedModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawIrregularCube() const
{
    if (!irregularCubeLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(120, 200, 180);

    // Move irregular cube to front-left
    glTranslatef(-20.0f, -18.7f, 20.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    irregularCubeModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}

void Environment::drawPillar() const
{
    if (!pillarLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(180, 180, 140);

    // Move pillar to front-right
    glTranslatef(20.0f, -18.7f, 15.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    pillarModel.draw();

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

void Environment::drawSphere() const
{
    if (!sphereLoaded)
        return;

    glPushMatrix();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    glColor3ub(255, 220, 120);

    // Move sphere to center-front so it is easy to see
    glTranslatef(0.0f, 0.0f, 10.0f);
    glScalef(8.0f, 8.0f, 8.0f);

    sphereModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();
}


/////////////////////////////////////Main draw function//////////////////////////
void Environment::draw() const
{
    drawSkyBox();
    drawGround();
    drawCastleWall();
    drawCube();
    drawCubeGrouped();
    drawIrregularCube();
    drawPillar();
    drawRoof();
    drawSphere();
}
