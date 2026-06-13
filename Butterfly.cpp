#include <GL/glut.h>
#include "Butterfly.hpp"

using namespace ProjectButterfly;

Butterfly::Butterfly()
{
    leftWingLoaded = false;
    rightWingLoaded = false;
    tursoLoaded = false;

    leftWingTextureID = 0;
    rightWingTextureID = 0;
    tursoTextureID = 0;
}

bool Butterfly::loadLeftWing(const std::string& filePath)
{
    leftWingLoaded = leftWingModel.loadFromObjText(filePath);
    return leftWingLoaded;
}

bool Butterfly::loadRightWing(const std::string& filePath)
{
    rightWingLoaded = rightWingModel.loadFromObjText(filePath);
    return rightWingLoaded;
}

bool Butterfly::loadTurso(const std::string& filePath)
{
    tursoLoaded = tursoModel.loadFromObjText(filePath);
    return tursoLoaded;
}

void Butterfly::drawLeftWing() const
{
    if (!leftWingLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leftWingTextureID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Change these values if Butterfly is too big / small / high / low
    glTranslatef(30.0f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    leftWingModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Butterfly::drawRightWing() const
{
    if (!rightWingLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, rightWingTextureID);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();

    // Must be same transform as left wing to keep model aligned
    glTranslatef(30.0f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    rightWingModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void Butterfly::drawTurso() const
{
    if (!tursoLoaded)
        return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tursoTextureID);

    glPushMatrix();

    // Must be same transform as wings to keep model aligned
    glTranslatef(30.0f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    glScalef(3.0f, 3.0f, 3.0f);

    glColor3f(1.0f, 1.0f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);

    tursoModel.draw();

    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void Butterfly::draw() const
{
    drawLeftWing();
    drawRightWing();
    drawTurso();
}
