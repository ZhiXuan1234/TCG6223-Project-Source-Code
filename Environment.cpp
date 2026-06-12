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
           skyTexture != 0;
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

    // Keep background texture bright
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, skyBoxTexture);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    // White color so texture is not tinted
    glColor3ub(255, 255, 255);

    // Size of the SkyBox
    glScalef(15.0f, 15.0f, 15.0f);

    skyBoxModel.draw();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    // Turn lighting back on for other objects
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

    // Size and translate of the roof
    glTranslatef(0.0f, 18.7f, 0.0f);
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

    // Cube left 1 - Cube1.png
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube1Texture);
    glTranslatef(-90.0f, -18.7f, 100.0f);
    glScalef(8.0f, 8.0f, 8.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube left 2 - Cube2.png
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube2Texture);
    glTranslatef(-180.0f, -18.7f, 40.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube right 1 - Cube2.png
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube2Texture);
    glTranslatef(90.0f, -18.7f, -130.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    cubeModel.draw();
    glPopMatrix();

    // Cube right 2 - use Cube1.png again
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, cube1Texture);
    glTranslatef(20.0f, -18.7f, -210.0f);
    glScalef(17.0f, 17.0f, 17.0f);
    cubeModel.draw();
    glPopMatrix();

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

    // Main floating ball above center
    glPushMatrix();
    glTranslatef(-10.0f, 50.0f, 10.0f);
    glScalef(9.0f, 9.0f, 9.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball left-back
    glPushMatrix();
    glTranslatef(-60.0f, 20.0f, -45.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball front-left
    glPushMatrix();
    glTranslatef(-90.0f, 30.0f, 90.0f);
    glScalef(20.0f, 20.0f, 20.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball right-back
    glPushMatrix();
    glTranslatef(75.0f, 35.0f, -60.0f);
    glScalef(15.0f, 15.0f, 15.0f);
    sphereModel.draw();
    glPopMatrix();

    // Floating ball right-front
    glPushMatrix();
    glTranslatef(85.0f, 25.0f, 100.0f);
    glScalef(10.0f, 10.0f, 10.0f);
    sphereModel.draw();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
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
